#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

int main() {

    std::string title = "Virtus in Morte";
    std::shared_ptr<Virtus::Window> window = std::make_shared<Virtus::Window>(
                                                    Virtus::Window::Extent{640, 480},
                                                    Virtus::Window::Position{0, 0},
                                                    title);
    std::shared_ptr<Virtus::Graphics> graphics = std::make_shared<Virtus::Graphics>(*window);
    Virtus::Context ctx(window, graphics);

    Virtus::Info("Hello, Virtus!");

    struct Vertex {
        float position[3];
        float color[3];
    };

    std::vector<Virtus::Graphics::BufferLayout::Member> layout_members = {
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0},
        {Virtus::Graphics::BufferLayout::Member::Type::FullFloat, 3, 0}
    };

    Vertex triangle[3] = {
        {{-0.5, -0.5, 0.0}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5, -0.5, 0.0}, {0.0f, 1.0f, 0.0f}},
        {{ 0.0,  0.5, 0.0}, {0.0f, 0.0f, 1.0f}}
    };

    Virtus::Graphics::BufferLayout layout(layout_members);
    Virtus::Graphics::VAO vao(layout);
    Virtus::Graphics::VBO& vbo = vao.CreateVBO(
                                        (void*) triangle,
                                        sizeof(triangle),
                                        Virtus::Graphics::BufferUsage::StaticDraw);

    std::string vertex_source = R"(
        #version 410 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 color;

        out vec3 vertex_color;

        void main() {
            gl_Position = vec4(position, 1.0);
            vertex_color = color;
        }
    )";
    std::string fragment_source = R"(
        #version 410 core

        in vec3 vertex_color;

        out vec4 color;

        void main() {
            color = vec4(vertex_color, 1.0f);
        }
    )";

    std::vector<Virtus::Graphics::Shader::Unit> units;
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Vertex, vertex_source));
    units.push_back(Virtus::Graphics::Shader::Unit(Virtus::Graphics::Shader::Unit::Stage::Fragment, fragment_source));

    Virtus::Graphics::Shader shader(units);

    while(!window->Poll()) {
        
        float color[3] = {0.8f, 0.4f, 0.3f};
        graphics->Clear(color);

        shader.Bind();
        vao.Bind();
        vbo.Bind();
        graphics->Draw(3, 1, Virtus::Graphics::DrawMode::Normal);

    }

}
