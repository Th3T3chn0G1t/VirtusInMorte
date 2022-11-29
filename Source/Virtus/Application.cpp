#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

int main(int argc, char** argv) {

    std::string resource_dir {"Resources"};
    if(argc > 1) {

        resource_dir = std::string(argv[1]);

    }

    // TODO: Pipeline state object to reduce GL calls
    //       Especially binding, which may currently involve redundant
    //       Re-binds

    std::string title {"Virtus in Morte"};
    Virtus::Window::Extent extent {640, 480};
    Virtus::Window window(extent, Virtus::Window::Position{0, 0}, title);
    window.SetCursorCapture(true);
    Virtus::Graphics graphics(window);
    Virtus::UI ui(window);

    Virtus::ImageLoader image_loader(resource_dir);
    Virtus::ShaderUnitLoader shader_unit_loader(resource_dir);
    Virtus::MeshLoader mesh_loader(resource_dir);
    Virtus::MaterialLoader material_loader(resource_dir);
    Virtus::UIStyleLoader ui_style_loader(resource_dir);

    std::string style_path {"default.ui.yaml"};
    ui.SetStyle(ui_style_loader.Get(style_path));

    std::string vertex_path {"instanced.vert.glsl"};
    std::string fragment_path {"fragment.frag.glsl"};
    Virtus::Graphics::Shader::Unit& vertex = shader_unit_loader.Get(vertex_path);
    Virtus::Graphics::Shader::Unit& fragment = shader_unit_loader.Get(fragment_path);
    Virtus::Graphics::Shader shader(vertex, fragment);

    std::string image_path {"test.bmp"};
    Virtus::Graphics::Texture texture(image_loader.Get(image_path), Virtus::Graphics::Texture::FilterMode::Linear, Virtus::Graphics::Texture::WrapMode::Clamp);

    glm::mat4 projection(glm::perspective(glm::radians(75.0f), (float) extent[0] / (float) extent[1], 0.01f, 100.0f));

    std::string map_path {fmt::format("{}/0.map.yaml", resource_dir)}; // TODO: Resource loader on maps
    Virtus::Map map(map_path, image_loader, shader_unit_loader, mesh_loader, material_loader);

    struct Camera {

        glm::vec3 m_Rotation;
        glm::vec3 m_Position;

    };

    Camera camera{{25.0f, 40.0f, 0.0f}, {2.0f, 0.0f, -1.0f}};

    glm::vec2 last_cursor(0.0f, 0.0f);
    bool captured = true;

    Common::Info("Hello, Virtus!");

    float health = 0.6f;
    bool inventory = false;
    bool i_held = false;

    while(!window.Poll()) {
        
        glm::vec2 cursor(window.GetCursor());

        glm::vec2 cursor_delta(cursor - last_cursor);

        last_cursor = cursor;

        bool i_state = window.GetKey(GLFW_KEY_I);
        if(!i_held && i_state) {
        
            captured = !captured;
            inventory = !inventory;

            window.SetCursorCapture(captured);

            i_held = true;

        }
        else if(!i_state) {

            i_held = false;

        }

        if(captured) {

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

        }

        glm::mat4 view(glm::mat4(1.0f));
        view = glm::rotate(view, camera.m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, camera.m_Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, camera.m_Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera.m_Position);

        graphics.Clear({0.8f, 0.4f, 0.3f});

        shader.Bind();

        int unit(0);
        texture.Bind(unit);

        std::string u_Texture("u_Texture");
        shader.Uniform(u_Texture, unit);

        std::string u_VP("u_VP");
        glm::mat4 VP(projection * view);
        shader.Uniform(u_VP, VP);

        std::string u_AmbientLight("u_AmbientLight");
        glm::vec3 ambient_light(0.2f, 0.2f, 0.2f);
        shader.Uniform(u_AmbientLight, ambient_light);

        std::string u_ViewPosition("u_ViewPosition");
        shader.Uniform(u_ViewPosition, camera.m_Position);

        std::string u_Time("u_Time");
        float time{static_cast<float>(glfwGetTime())};
        shader.Uniform(u_Time, time);

        std::string u_Viewport("u_Viewport");
        glm::vec2 viewport(extent[0], extent[1]);
        shader.Uniform(u_Viewport, viewport);

        map.Draw(graphics, shader);

        ui.Begin();

        ui.SetBackground(false);

        if(nk_begin(ui, "HealthBar", nk_rect(0, 0, 640, 480), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {

            nk_layout_row_static(ui, 10, 640, 1);
            Common::usz health_scaled = static_cast<Common::usz>(640 * health);
            nk_progress(ui, &health_scaled, 640, NK_FIXED);

            nk_end(ui);
        }

        ui.SetBackground(true);

        if(inventory && nk_begin(ui, "Inventory", nk_rect(64, 64, 512, 352), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_BACKGROUND)) {

            nk_end(ui);
        }

        ui.End();

    }

}
