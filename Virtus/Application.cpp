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
    std::shared_ptr<Virtus::Window> window = std::make_shared<Virtus::Window>(
                                                    extent,
                                                    Virtus::Window::Position{0, 0},
                                                    title);
    window->SetCursorCapture(true);
    std::shared_ptr<Virtus::Graphics> graphics = std::make_shared<Virtus::Graphics>(*window);
    Virtus::Context ctx(window, graphics);

    Virtus::Info("Hello, Virtus!");

    struct Vertex {
        glm::vec3 m_Position;
        glm::vec4 m_Color;
        glm::vec2 m_UV;
    };

    std::vector<Virtus::Graphics::BufferLayout::Member> generic_layout_members {
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 2, 0}
    };

    Vertex mesh[] {
        {{-0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    Virtus::Graphics::BufferLayout generic_layout(generic_layout_members);
    Virtus::Graphics::VAO vao;
    vao.CreateVBO((void*) mesh, sizeof(mesh), Virtus::Graphics::BufferUsage::StaticDraw, generic_layout);

    struct InstanceData {
        glm::mat4 position;
    };

    InstanceData instance_data[] = {
        {glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -1.0f})},
        {glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 1.0f})}
    };

    std::vector<Virtus::Graphics::BufferLayout::Member> instance_layout_members {
        // TODO: Special case for matrices
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 4, 1},
    };
    Virtus::Graphics::BufferLayout instance_layout(instance_layout_members);
    vao.CreateVBO((void*) instance_data, sizeof(instance_data), Virtus::Graphics::BufferUsage::StaticDraw, instance_layout);

    std::vector<uint> indices {
        0, 1, 2,
        2, 3, 1
    };

    Virtus::Graphics::IBO ibo(indices, Virtus::Graphics::BufferUsage::StaticDraw);

    std::string vertex_source = R"(
        #version 410 core

        layout(location = 0) in vec3 v_Position;
        layout(location = 1) in vec4 v_Color;
        layout(location = 2) in vec2 v_UV;

        layout(location = 3) in mat4 i_Transform;

        uniform mat4 u_VP;

        out vec4 o_Color;
        out vec2 o_UV;

        void main() {
            gl_Position = u_VP * i_Transform * vec4(v_Position, 1.0);
            o_Color = v_Color;
            o_UV = v_UV;
        }
    )";
    std::string fragment_source = R"(
        #version 410 core

        in vec4 o_Color;
        in vec2 o_UV;

        uniform sampler2D u_Texture;

        out vec4 f_Color;

        void main() {
            f_Color = texture(u_Texture, o_UV) * o_Color;
        }
    )";

    std::vector<Virtus::Graphics::Shader::Unit> units;
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Vertex, vertex_source));
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Fragment, fragment_source));

    Virtus::Graphics::Shader shader(units);

#ifdef __INTELLISENSE
    std::string path = fmt::format("{}/test.bmp", resource_dir);
#else
    std::filesystem::path path = std::filesystem::path(fmt::format("{}/test.bmp", resource_dir));
#endif
    Virtus::Graphics::Image image(path);
    Virtus::Graphics::Texture texture(image, Virtus::Graphics::Texture::FilterMode::Linear, Virtus::Graphics::Texture::WrapMode::Clamp);

    glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float) extent[0] / (float) extent[1], 0.01f, 100.0f);

    glm::vec2 last_cursor(0.0f, 0.0f);

    struct Camera {
        glm::vec3 m_Rotation;
        glm::vec3 m_Position;
    };

    Camera camera{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

    while(!window->Poll()) {

        glm::vec2 cursor = window->GetCursor();

        glm::vec2 cursor_delta = cursor - last_cursor;

        last_cursor = cursor;

        graphics->Clear({0.8f, 0.4f, 0.3f});

        shader.Bind();
        vao.Bind();
        ibo.Bind();

        int unit = 0;
        texture.Bind(unit);

        std::string u_Texture = "u_Texture";
        shader.Uniform(u_Texture, unit);

        camera.m_Rotation.x += glm::radians(cursor_delta.x);
        camera.m_Rotation.y += glm::radians(cursor_delta.y);

        float movement_forward = 0.0f;
        float movement_right = 0.0f;
        if(window->GetKey(GLFW_KEY_W)) movement_right = -0.1f;
        if(window->GetKey(GLFW_KEY_S)) movement_right = 0.1f;
        if(window->GetKey(GLFW_KEY_D)) movement_forward = 0.1f;
        if(window->GetKey(GLFW_KEY_A)) movement_forward = -0.1f;

        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 forward(glm::cos(camera.m_Rotation.x), 0.0f, glm::sin(camera.m_Rotation.x));
        glm::vec3 right(glm::cross(forward, up));

        camera.m_Position += (movement_forward * forward) + (movement_right * right);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, camera.m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, camera.m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera.m_Position);

        std::string u_VP = "u_VP";
        glm::mat4 VP = projection * view;
        shader.Uniform(u_VP, VP);

        graphics->Draw(6, 2, Virtus::Graphics::DrawMode::Indexed);

    }

}
