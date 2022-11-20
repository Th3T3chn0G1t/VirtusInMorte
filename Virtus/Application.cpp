#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

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

    struct Vertex {
        glm::vec3 m_Position;
        glm::vec4 m_Color;
        glm::vec2 m_UV;
        glm::vec3 m_Normal;
    };

    std::vector<Virtus::Graphics::BufferLayout::Member> generic_layout_members {
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 2, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0}
    };

    Vertex mesh[] {
        {{-0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}
    };

    Virtus::Graphics::BufferLayout generic_layout(generic_layout_members);
    Virtus::Graphics::VAO vao;
    vao.CreateVBO((void*) mesh, sizeof(mesh), Virtus::Graphics::BufferUsage::StaticDraw, generic_layout);

    class InstanceData {

    public:
        glm::mat4 m_Transform;
        glm::mat3 m_Normal;
        uint m_Specular;

    public:
        InstanceData(glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scale = {1.0f, 1.0f, 1.0f}, uint specular = 0) : m_Specular(specular) {

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

    std::vector<Virtus::Graphics::BufferLayout::Member> instance_layout_members {
        // TODO: Special case for matrices
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},

        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 1},

        {Virtus::Graphics::BufferLayout::Member::Type::UnsignedInt, 1, 1}
    };
    Virtus::Graphics::BufferLayout instance_layout(instance_layout_members);
    vao.CreateVBO((void*) instance_data, sizeof(instance_data), Virtus::Graphics::BufferUsage::StaticDraw, instance_layout);

    std::vector<uint> indices {
        0, 1, 2,
        1, 3, 2
    };

    Virtus::Graphics::IBO ibo(indices, Virtus::Graphics::BufferUsage::StaticDraw);

    std::string vertex_source;
    if(std::ifstream stream {fmt::format("{}/vertex.glsl", resource_dir), std::ios::binary | std::ios::ate}) {
        auto size(stream.tellg());
        vertex_source.resize(size);
        stream.seekg(0);
#ifndef __INTELLISENSE
        stream.read(vertex_source.data(), size);
#endif
    }

    std::string fragment_source;
    if(std::ifstream stream {fmt::format("{}/fragment.glsl", resource_dir), std::ios::binary | std::ios::ate}) {
        auto size(stream.tellg());
        fragment_source.resize(size);
        stream.seekg(0);
#ifndef __INTELLISENSE
        stream.read(fragment_source.data(), size);
#endif
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

        graphics->Draw(6, sizeof(instance_data) / sizeof(InstanceData), Virtus::Graphics::DrawMode::Indexed);

    }

}
