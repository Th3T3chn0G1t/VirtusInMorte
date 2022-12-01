using namespace Common;

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
    Virtus::MapLoader map_loader(resource_dir);
    Virtus::ResourceLoaderBundle loader_bundle{image_loader, shader_unit_loader, mesh_loader, material_loader, map_loader};
    Virtus::UIStyleLoader ui_style_loader(resource_dir);

    std::string style_path {"default.ui.yaml"};
    ui.SetStyle(ui_style_loader.Get(style_path, loader_bundle));

    std::string vertex_path {"instanced.vert.glsl"};
    std::string fragment_path {"fragment.frag.glsl"};
    Virtus::Graphics::Shader::Unit& vertex = shader_unit_loader.Get(vertex_path, loader_bundle);
    Virtus::Graphics::Shader::Unit& fragment = shader_unit_loader.Get(fragment_path, loader_bundle);
    Virtus::Graphics::Shader shader(vertex, fragment);

    std::string image_path {"test.bmp"};
    Virtus::Graphics::Texture texture(image_loader.Get(image_path, loader_bundle), Virtus::Graphics::Texture::FilterMode::Linear, Virtus::Graphics::Texture::WrapMode::Clamp);

    glm::mat4 projection(glm::perspective(glm::radians(75.0f), (float) extent[0] / (float) extent[1], 0.01f, 100.0f));

    std::string map_path {"0.map.yaml"}; // TODO: Resource loader on maps
    Virtus::Map& map(map_loader.Get(map_path, loader_bundle));

    struct Camera {

        glm::vec3 m_Rotation;
        glm::vec3 m_Position;

    };

    Camera camera{{25.0f, 40.0f, 0.0f}, {2.0f, 0.0f, -1.0f}};

    glm::vec2 last_cursor(0.0f, 0.0f);
    bool captured = true;

    std::minstd_rand rng(std::chrono::system_clock::now().time_since_epoch().count());

    Info("Hello, Virtus!");

    float health = 0.6f;
    bool inventory = false;
    bool i_held = false;
    bool g_held = false;

    std::string none = "None";
    std::string armor = "Armor";
    std::string weapon = "Weapon";
    Item armor_nothing{Item::Usage::Equipable, Item::Slot::Armor, Item::Rarity::Common, none, armor};
    Item weapon_nothing{Item::Usage::Equipable, Item::Slot::Weapon, Item::Rarity::Common, none, weapon};
    std::optional<Item> armor_slot;
    std::optional<Item> weapon_slot;

    std::vector<Item> items;

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

        bool g_state = window.GetKey(GLFW_KEY_G);
        if(!g_held && g_state) {

            uchar val = rng();
            float d = static_cast<float>(val) / 255.0f;
            float scaled = d * 3.0f;
            auto n = static_cast<uchar>(scaled);
            switch(n) {

                case 0: {

                    std::string name = "Ragged Cloak";
                    std::string description = "Just something to keep the draught out";
                    items.emplace_back(Item::Usage::Equipable, Item::Slot::Armor, Item::Rarity::Common, name, description);
                    break;

                }

                case 1: {

                    std::string name = "Shadow Blade";
                    std::string description = "It's basically just a black stick";
                    items.emplace_back(Item::Usage::Equipable, Item::Slot::Weapon, Item::Rarity::Common, name, description);
                    break;

                }

                case 2: {

                    std::string name = "Rock";
                    std::string description = "Literally a rock";
                    items.emplace_back(Item::Usage::Reusable, Item::Slot::None, Item::Rarity::Common, name, description);
                    break;

                }

                default: Fatal("Something went wrong!");

            }

            g_held = true;

        }
        else if(!g_state) {

            g_held = false;

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

        ui.SetBackground(true);

        if(inventory && nk_begin(ui, "Inventory", nk_rect(64, 64, 512, 352), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_BACKGROUND)) {

            nk_layout_row_static(ui, 64.0f, 128.0f, 6);

            {

                struct nk_rect bounds = nk_widget_bounds(ui);
                if(nk_button_label(ui, armor_slot.value_or(armor_nothing).m_Name.c_str()) && armor_slot.has_value()) {

                    armor_slot.value().OnDisarm();
                    items.push_back(armor_slot.value());
                    armor_slot.reset();

                }
                if(nk_input_is_mouse_hovering_rect(&((nk_context*) ui)->input, bounds)) {

                    nk_tooltip(ui, armor_slot.value_or(armor_nothing).m_Description.c_str());

                }

                bounds = nk_widget_bounds(ui);
                if(nk_button_label(ui, weapon_slot.value_or(weapon_nothing).m_Name.c_str()) && weapon_slot) {

                    weapon_slot.value().OnDisarm();
                    items.push_back(weapon_slot.value());
                    weapon_slot.reset();

                }
                if(nk_input_is_mouse_hovering_rect(&((nk_context*) ui)->input, bounds)) {

                    nk_tooltip(ui, weapon_slot.value_or(weapon_nothing).m_Description.c_str());

                }


            }

            nk_layout_row_dynamic(ui, 64.0f, 6);
            std::optional<usz> to_remove;
            for(usz i = 0; i < items.size(); ++i) {

                struct nk_rect bounds = nk_widget_bounds(ui);
                if(nk_button_label(ui, items[i].m_Name.c_str())) {

                    items[i].OnUse();
                    switch(items[i].m_Usage) {

                        case Item::Usage::Consumable: {

                            to_remove = i;
                            break;

                        }
                        case Item::Usage::Reusable: {

                            break;

                        }
                        case Item::Usage::Equipable: {

                            switch(items[i].m_Slot) {

                                case Item::Slot::None: {

                                    Fatal(fmt::format("Attempt to equip unequipable item `{}`", items[i].m_Name));

                                }
                                case Item::Slot::Weapon: {

                                    if(weapon_slot) {

                                        items.push_back(weapon_slot.value());
                                        weapon_slot.value().OnDisarm();

                                    }

                                    weapon_slot = items[i];
                                    to_remove = i;

                                    break;

                                }
                                case Item::Slot::Armor: {

                                    if(armor_slot) {

                                        items.push_back(armor_slot.value());
                                        armor_slot.value().OnDisarm();

                                    }

                                    armor_slot = items[i];
                                    to_remove = i;

                                    break;

                                }

                            }

                            break;

                        }

                    }

                }

                if(nk_input_is_mouse_hovering_rect(&((nk_context*) ui)->input, bounds)) {

                    nk_tooltip(ui, items[i].m_Description.c_str());

                }

            }

            if(to_remove) {

                items.erase(items.begin() + static_cast<long>(to_remove.value()));

            }

            nk_end(ui);
        }

        ui.SetBackground(false);

        if(!inventory && nk_begin(ui, "HealthBar", nk_rect(0, 0, 640, 480), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {

            nk_layout_row_static(ui, 10, 640, 1);
            usz health_scaled = static_cast<usz>(640 * health);
            nk_progress(ui, &health_scaled, 640, NK_FIXED);

            nk_end(ui);
        }

        ui.End();

    }

}
