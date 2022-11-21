#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

struct Vertex {

    glm::vec3 m_Position;
    glm::vec4 m_Color;
    glm::vec2 m_UV;
    glm::vec3 m_Normal;

};

class InstanceData {

public:
    glm::mat4 m_Transform;
    glm::mat3 m_Normal;
    uint m_Specular;
    uint m_DoSample;

public:
    InstanceData(glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scale = {1.0f, 1.0f, 1.0f}, uint specular = 0, uint do_sample = true) : m_Specular(specular), m_DoSample(do_sample) {

        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
        transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
        transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, scale);

        m_Transform = transform;
        m_Normal = glm::mat3(glm::transpose(glm::inverse(transform)));

    }

};

int main(int argc, char** argv) {

    std::string resource_dir = "Resources";
    if(argc > 1) {

        resource_dir = std::string(argv[1]);

    }

    std::string title = "Virtus in Morte";
    Virtus::Window::Extent extent {640, 480};
    std::shared_ptr<Virtus::Window> window(std::make_shared<Virtus::Window>(
                                                    extent,
                                                    Virtus::Window::Position{0, 0},
                                                    title));
    window->SetCursorCapture(true);
    std::shared_ptr<Virtus::Graphics> graphics(std::make_shared<Virtus::Graphics>(*window));
    Virtus::Context ctx(window, graphics);

    Virtus::Info("Hello, Virtus!");

    std::vector<Virtus::Graphics::BufferLayout::Member> generic_layout_members {
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 2, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0}
    };
    Virtus::Graphics::BufferLayout generic_layout(generic_layout_members);

    std::vector<Virtus::Graphics::BufferLayout::Member> instance_layout_members {
        // TODO: Special case for matrices
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},

        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},

        {Virtus::Graphics::BufferLayout::Member::Type::UnsignedInt, 1, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::UnsignedInt, 1, 1}
    };
    Virtus::Graphics::BufferLayout instance_layout(instance_layout_members);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fmt::format("{}/test.obj", resource_dir),
                                      aiProcess_Triangulate
                                    | aiProcess_JoinIdenticalVertices 
                                    | aiProcess_SortByPType);

    if(!scene) {

        Virtus::Fatal(fmt::format("{}", importer.GetErrorString()));

    }

    const aiMesh* loaded = scene->mMeshes[0];

    std::vector<Vertex> loaded_mesh;

    for(Virtus::usz i = 0; i < loaded->mNumVertices; ++i) {

        glm::vec3 position{loaded->mVertices[i].x, loaded->mVertices[i].y, loaded->mVertices[i].z};
        glm::vec4 color{loaded->mColors[0] ?
            glm::vec4{loaded->mColors[0][i].r, loaded->mColors[0][i].g, loaded->mColors[0][i].b, loaded->mColors[0][i].a} :
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
        };
        glm::vec2 UV{loaded->mTextureCoords[0] ?
                glm::vec2{loaded->mTextureCoords[0][i].x, loaded->mTextureCoords[0][i].y} :
                glm::vec2{0.0f, 0.0f}};
        glm::vec3 normal{loaded->mNormals[i].x, loaded->mNormals[i].y, loaded->mNormals[i].z};

        loaded_mesh.push_back({position, color, UV, normal});

    }

    std::vector<uint> loaded_indices;

    for(Virtus::usz i = 0; i < loaded->mNumFaces; ++i) {

        Virtus::usz old = loaded_indices.size();
        loaded_indices.resize(loaded_indices.size() + loaded->mFaces[i].mNumIndices);
        std::memcpy(
                &loaded_indices.data()[old],
                loaded->mFaces[i].mIndices,
                loaded->mFaces[i].mNumIndices * sizeof(uint));

    }

    Virtus::Graphics::IBO loaded_ibo(loaded_indices, Virtus::Graphics::BufferUsage::StaticDraw);

    Virtus::Graphics::VAO loaded_vao;
    loaded_vao.CreateVBO((void*) loaded_mesh.data(), loaded_mesh.size() * sizeof(Vertex), Virtus::Graphics::BufferUsage::StaticDraw, generic_layout);

    Vertex mesh[] {

        {{-0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}

    };

    InstanceData loaded_instance_data[] {

        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, 16, false},

    };
    loaded_vao.CreateVBO((void*) loaded_instance_data, sizeof(loaded_instance_data), Virtus::Graphics::BufferUsage::StaticDraw, instance_layout);

    Virtus::Graphics::VAO vao;
    vao.CreateVBO((void*) mesh, sizeof(mesh), Virtus::Graphics::BufferUsage::StaticDraw, generic_layout);

    InstanceData instance_data[] {

        {{-3.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-1.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{0.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{1.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{3.0f, 0.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 16},

        {{-3.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-1.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{0.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{1.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{3.0f, 0.0f, 2.0f}, {0.0f, glm::radians(180.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},

        {{-2.0f, 0.0f, -3.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, -2.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, -1.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, 0.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, 1.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, 2.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{-2.0f, 0.0f, 3.0f}, {0.0f, glm::radians(90.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},

        {{2.0f, 0.0f, -3.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, -2.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, -1.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, 0.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, 1.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, 2.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16},
        {{2.0f, 0.0f, 3.0f}, {0.0f, glm::radians(270.0f), 0.0f}, {1.0f, 1.0f, 1.0f}, 16}

    };

    vao.CreateVBO((void*) instance_data, sizeof(instance_data), Virtus::Graphics::BufferUsage::StaticDraw, instance_layout);

    std::vector<uint> indices {
        0, 1, 2,
        1, 3, 2
    };

    Virtus::Graphics::IBO ibo(indices, Virtus::Graphics::BufferUsage::StaticDraw);

    std::string vertex_source;
    if(std::ifstream stream {fmt::format("{}/vertex.vert.glsl", resource_dir), std::ios::binary | std::ios::ate}) {
        auto size(stream.tellg());
        vertex_source.resize(size);
        stream.seekg(0);
#ifndef __INTELLISENSE
        stream.read(vertex_source.data(), size);
#endif
    }
    else {

        Virtus::Fatal(fmt::format("Failed to load shader from `{}/vertex.vert.glsl`", resource_dir));

    }

    std::string fragment_source;
    if(std::ifstream stream {fmt::format("{}/fragment.frag.glsl", resource_dir), std::ios::binary | std::ios::ate}) {
        auto size(stream.tellg());
        fragment_source.resize(size);
        stream.seekg(0);
#ifndef __INTELLISENSE
        stream.read(fragment_source.data(), size);
#endif
    }
    else {

        Virtus::Fatal(fmt::format("Failed to load shader from `{}/fragment.frag.glsl`", resource_dir));

    }

    std::vector<Virtus::Graphics::Shader::Unit> units;
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Vertex, vertex_source));
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Fragment, fragment_source));

    Virtus::Graphics::Shader shader(units);

#ifdef __INTELLISENSE
    std::string path(fmt::format("{}/test.bmp", resource_dir));
#else
    std::filesystem::path path(std::filesystem::path(fmt::format("{}/test.bmp", resource_dir)));
#endif
    Virtus::Graphics::Image image(path);
    Virtus::Graphics::Texture texture(image, Virtus::Graphics::Texture::FilterMode::Linear, Virtus::Graphics::Texture::WrapMode::Clamp);

    glm::mat4 projection(glm::perspective(glm::radians(75.0f), (float) extent[0] / (float) extent[1], 0.01f, 100.0f));

    glm::vec2 last_cursor(0.0f, 0.0f);

    struct Camera {
        glm::vec3 m_Rotation;
        glm::vec3 m_Position;
    };

    Camera camera{{25.0f, 40.0f, 0.0f}, {2.0f, 0.0f, -1.0f}};

    while(!window->Poll()) {

        glm::vec2 cursor(window->GetCursor());

        glm::vec2 cursor_delta(cursor - last_cursor);

        last_cursor = cursor;

        graphics->Clear({0.8f, 0.4f, 0.3f});

        shader.Bind();
        vao.Bind();
        ibo.Bind();

        int unit(0);
        texture.Bind(unit);

        std::string u_Texture("u_Texture");
        shader.Uniform(u_Texture, unit);

        camera.m_Rotation.x += glm::radians(cursor_delta.x);
        camera.m_Rotation.y += glm::radians(cursor_delta.y);

        float movement_forward(0.0f);
        float movement_right(0.0f);
        if(window->GetKey(GLFW_KEY_W)) movement_right = -0.1f;
        if(window->GetKey(GLFW_KEY_S)) movement_right = 0.1f;
        if(window->GetKey(GLFW_KEY_D)) movement_forward = 0.1f;
        if(window->GetKey(GLFW_KEY_A)) movement_forward = -0.1f;

        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 forward(glm::cos(camera.m_Rotation.x), 0.0f, glm::sin(camera.m_Rotation.x));
        glm::vec3 right(glm::cross(forward, up));

        camera.m_Position += (movement_forward * forward) + (movement_right * right);

        glm::mat4 view(glm::mat4(1.0f));
        view = glm::rotate(view, camera.m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, camera.m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, camera.m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera.m_Position);

        std::string u_VP("u_VP");
        glm::mat4 VP(projection * view);
        shader.Uniform(u_VP, VP);

        std::string u_AmbientLight("u_AmbientLight");
        glm::vec3 ambient_light(0.2f, 0.2f, 0.2f);
        shader.Uniform(u_AmbientLight, ambient_light);

        std::string u_ViewPosition("u_ViewPosition");
        shader.Uniform(u_ViewPosition, camera.m_Position);

        std::string u_Specular("u_Specular");
        float specular{0.5f};
        shader.Uniform(u_Specular, specular);

        std::string u_Time("u_Time");
        float time{static_cast<float>(glfwGetTime())};
        shader.Uniform(u_Time, time);

        std::string u_Viewport("u_Viewport");
        glm::vec2 viewport(extent[0], extent[1]);
        shader.Uniform(u_Viewport, viewport);

        graphics->Draw(indices.size(), sizeof(instance_data) / sizeof(InstanceData), Virtus::Graphics::DrawMode::Indexed);

        loaded_vao.Bind();
        loaded_ibo.Bind();
        graphics->Draw(loaded_indices.size(), 1, Virtus::Graphics::DrawMode::Indexed);

    }

}
