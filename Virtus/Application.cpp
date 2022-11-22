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
    Virtus::Window window(extent, Virtus::Window::Position{0, 0}, title);
    window.SetCursorCapture(true);
    Virtus::Graphics graphics(window);

    Virtus::ImageLoader image_loader(resource_dir);
    Virtus::ShaderUnitLoader shader_unit_loader(resource_dir);
    Virtus::MeshLoader mesh_loader(resource_dir);

    std::vector<Virtus::Graphics::Vertex> mesh {

        {{-0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}

    };

    std::vector<uint> indices {
        0, 1, 2,
        1, 3, 2
    };

    Virtus::Graphics::IBO ibo(indices, Virtus::Graphics::BufferUsage::StaticDraw);

    std::vector<Virtus::Graphics::InstanceData> instance_data {

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

    Virtus::Graphics::VAO vao;
    vao.CreateVBO(mesh, Virtus::Graphics::BufferUsage::StaticDraw, Virtus::Graphics::Vertex::Layout);
    vao.CreateVBO(instance_data, Virtus::Graphics::BufferUsage::StaticDraw, Virtus::Graphics::InstanceData::Layout);

    std::string instanced_vertex_path = "instanced.vert.glsl";
    std::string fragment_path = "fragment.frag.glsl";
    Virtus::Graphics::Shader::Unit& instanced_vertex = shader_unit_loader.Get(instanced_vertex_path);
    Virtus::Graphics::Shader::Unit& fragment = shader_unit_loader.Get(fragment_path);
    Virtus::Graphics::Shader instanced_shader(instanced_vertex, fragment);

    std::string image_path = "test.bmp";
    Virtus::Graphics::Texture texture(image_loader.Get(image_path), Virtus::Graphics::Texture::FilterMode::Linear, Virtus::Graphics::Texture::WrapMode::Clamp);

    std::string mesh_path = "test.obj";
    Virtus::Graphics::Mesh& loaded_mesh = mesh_loader.Get(mesh_path);
    std::vector<Virtus::Graphics::InstanceData> loaded_instance_data {

        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, 16, 0.8f, false}

    };
    loaded_mesh.m_Elements[0].m_VAO.CreateVBO(loaded_instance_data, Virtus::Graphics::BufferUsage::StaticDraw, Virtus::Graphics::InstanceData::Layout);

    glm::mat4 projection(glm::perspective(glm::radians(75.0f), (float) extent[0] / (float) extent[1], 0.01f, 100.0f));

    glm::vec2 last_cursor(0.0f, 0.0f);

    struct Camera {

        glm::vec3 m_Rotation;
        glm::vec3 m_Position;

    };

    Camera camera{{25.0f, 40.0f, 0.0f}, {2.0f, 0.0f, -1.0f}};

    Virtus::Info("Hello, Virtus!");

    while(!window.Poll()) {

        glm::vec2 cursor(window.GetCursor());

        glm::vec2 cursor_delta(cursor - last_cursor);

        last_cursor = cursor;

        camera.m_Rotation.x += glm::radians(cursor_delta.x);
        camera.m_Rotation.y += glm::radians(cursor_delta.y);

        float movement_forward(0.0f);
        float movement_right(0.0f);
        if(window.GetKey(GLFW_KEY_W)) movement_right = -0.1f;
        if(window.GetKey(GLFW_KEY_S)) movement_right = 0.1f;
        if(window.GetKey(GLFW_KEY_D)) movement_forward = 0.1f;
        if(window.GetKey(GLFW_KEY_A)) movement_forward = -0.1f;

        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 forward(glm::cos(camera.m_Rotation.x), 0.0f, glm::sin(camera.m_Rotation.x));
        glm::vec3 right(glm::cross(forward, up));

        camera.m_Position += (movement_forward * forward) + (movement_right * right);

        glm::mat4 view(glm::mat4(1.0f));
        view = glm::rotate(view, camera.m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, camera.m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, camera.m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera.m_Position);

        graphics.Clear({0.8f, 0.4f, 0.3f});

        instanced_shader.Bind();
        vao.Bind();
        ibo.Bind();

        int unit(0);
        texture.Bind(unit);

        std::string u_Texture("u_Texture");
        instanced_shader.Uniform(u_Texture, unit);

        std::string u_VP("u_VP");
        glm::mat4 VP(projection * view);
        instanced_shader.Uniform(u_VP, VP);

        std::string u_AmbientLight("u_AmbientLight");
        glm::vec3 ambient_light(0.2f, 0.2f, 0.2f);
        instanced_shader.Uniform(u_AmbientLight, ambient_light);

        std::string u_ViewPosition("u_ViewPosition");
        instanced_shader.Uniform(u_ViewPosition, camera.m_Position);

        std::string u_Time("u_Time");
        float time{static_cast<float>(glfwGetTime())};
        instanced_shader.Uniform(u_Time, time);

        std::string u_Viewport("u_Viewport");
        glm::vec2 viewport(extent[0], extent[1]);
        instanced_shader.Uniform(u_Viewport, viewport);

        std::string u_LightCount("u_LightCount");
        uint light_count = 2;
        instanced_shader.Uniform(u_LightCount, light_count);

        std::string u_LightPositions("u_LightPositions");
        std::vector<glm::vec3> light_positions {
            {1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f}
        };
        instanced_shader.Uniform(u_LightPositions, light_positions);

        std::string u_LightColors("u_LightColors");
        std::vector<glm::vec3> light_colors {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f},
        };
        instanced_shader.Uniform(u_LightColors, light_colors);

        graphics.Draw(indices.size(), instance_data.size(), Virtus::Graphics::DrawMode::Indexed);

        loaded_mesh.m_Elements[0].Bind();

        graphics.Draw(loaded_mesh.m_Elements[0].m_IndexCount, 1, Virtus::Graphics::DrawMode::Indexed);

    }

}
