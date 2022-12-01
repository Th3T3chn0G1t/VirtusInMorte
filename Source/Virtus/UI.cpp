namespace Virtus {

    UI::Style::Style(std::string& path) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        YAML::Node map = YAML::LoadFile(path);
        
        m_Border = {
            map["border"][0].as<uint>(),
            map["border"][1].as<uint>(),
            map["border"][2].as<uint>(),
            map["border"][3].as<uint>()};

        m_Background = {
            map["background"][0].as<uint>(),
            map["background"][1].as<uint>(),
            map["background"][2].as<uint>(),
            map["background"][3].as<uint>()};

        m_Content = {
            map["content"][0].as<uint>(),
            map["content"][1].as<uint>(),
            map["content"][2].as<uint>(),
            map["content"][3].as<uint>()};

        m_Inactive = {
            map["inactive"][0].as<uint>(),
            map["inactive"][1].as<uint>(),
            map["inactive"][2].as<uint>(),
            map["inactive"][3].as<uint>()};

        m_Element = {
            map["element"][0].as<uint>(),
            map["element"][1].as<uint>(),
            map["element"][2].as<uint>(),
            map["element"][3].as<uint>()};

        m_Active = {
            map["active"][0].as<uint>(),
            map["active"][1].as<uint>(),
            map["active"][2].as<uint>(),
            map["active"][3].as<uint>()};

    }


    void UI::SetStyle(Style& style) {

        m_Style = style;

        nk_color border = nk_rgba(style.m_Border.r, style.m_Border.g, style.m_Border.b, style.m_Border.a);
        nk_color background = nk_rgba(style.m_Background.r, style.m_Background.g, style.m_Background.b, style.m_Background.a);
        nk_color content = nk_rgba(style.m_Content.r, style.m_Content.g, style.m_Content.b, style.m_Content.a);
        nk_color inactive = nk_rgba(style.m_Inactive.r, style.m_Inactive.g, style.m_Inactive.b, style.m_Inactive.a);
        nk_color element = nk_rgba(style.m_Element.r, style.m_Element.g, style.m_Element.b, style.m_Element.a);
        nk_color active = nk_rgba(style.m_Active.r, style.m_Active.g, style.m_Active.b, style.m_Active.a);

        nk_style_item border_item = nk_style_item_color(border);
        nk_style_item background_item = nk_style_item_color(background);
        nk_style_item content_item = nk_style_item_color(content);
        nk_style_item inactive_item = nk_style_item_color(inactive);
        nk_style_item element_item = nk_style_item_color(element);
        nk_style_item active_item = nk_style_item_color(active);

        m_Context->style.window.fixed_background = background_item;
        m_Context->style.window.background = background;
        m_Context->style.window.border_color = border;
        m_Context->style.window.combo_border_color = border;
        m_Context->style.window.contextual_border_color = border;
        m_Context->style.window.menu_border_color = border;
        m_Context->style.window.group_border_color = border;
        m_Context->style.window.tooltip_border_color = border;
        m_Context->style.window.border_color = border;
        m_Context->style.window.scrollbar_size = nk_vec2(16.0f, 16.0f);
        m_Context->style.window.padding = nk_vec2(4.0f, 4.0f);
        m_Context->style.window.border = 1.0f;

        m_Context->style.window.header.label_normal = content;
        m_Context->style.window.header.label_hover = content;
        m_Context->style.window.header.label_active = content;
        m_Context->style.window.header.active = active_item;
        m_Context->style.window.header.hover = active_item;
        m_Context->style.window.header.normal = active_item;

        m_Context->style.scrollv.normal = background_item;
        m_Context->style.scrollv.hover = background_item;
        m_Context->style.scrollv.active = background_item;
        m_Context->style.scrollv.cursor_normal = content_item;
        m_Context->style.scrollv.cursor_hover = content_item;
        m_Context->style.scrollv.cursor_active = content_item;
        m_Context->style.scrollv.border_color = border;
        m_Context->style.scrollv.cursor_border_color = border;
        
        m_Context->style.scrollv.dec_symbol = NK_SYMBOL_NONE;
        m_Context->style.scrollv.inc_symbol = NK_SYMBOL_NONE;
        m_Context->style.scrollv.show_buttons = false;
        m_Context->style.scrollv.border = 1.0f;
        m_Context->style.scrollv.rounding = 0.0f;
        m_Context->style.scrollv.border_cursor = 1.0f;
        m_Context->style.scrollv.rounding_cursor = 0.0f;

        m_Context->style.checkbox.normal = inactive_item;
        m_Context->style.checkbox.hover = inactive_item;
        m_Context->style.checkbox.active = active_item;
        m_Context->style.checkbox.cursor_normal = content_item;
        m_Context->style.checkbox.cursor_hover = content_item;
        m_Context->style.checkbox.text_normal = content;
        m_Context->style.checkbox.text_hover = content;
        m_Context->style.checkbox.text_active = content;
        m_Context->style.checkbox.border_color = border;
        m_Context->style.checkbox.border = 1.0f;

        m_Context->style.option.normal = inactive_item;
        m_Context->style.option.hover = inactive_item;
        m_Context->style.option.active = active_item;
        m_Context->style.option.cursor_normal = content_item;
        m_Context->style.option.cursor_hover = content_item;
        m_Context->style.option.text_normal = content;
        m_Context->style.option.text_hover = content;
        m_Context->style.option.text_active = content;
        m_Context->style.option.border_color = border;
        m_Context->style.option.border = 1.0f;

        m_Context->style.button.normal = inactive_item;
        m_Context->style.button.hover = inactive_item;
        m_Context->style.button.active = active_item;
        m_Context->style.button.border_color = border;
        m_Context->style.button.text_background = background;
        m_Context->style.button.text_normal = content;
        m_Context->style.button.text_hover = content;
        m_Context->style.button.text_active = content;
        m_Context->style.button.rounding = 0.0f;

        m_Context->style.text.color = content;

        m_Context->style.contextual_button.normal = background_item;
        m_Context->style.contextual_button.hover = active_item;
        m_Context->style.contextual_button.active = background_item;
        m_Context->style.contextual_button.border_color = border;
        m_Context->style.contextual_button.text_background = background;
        m_Context->style.contextual_button.text_normal = content;
        m_Context->style.contextual_button.text_hover = content;
        m_Context->style.contextual_button.text_active = content;

        m_Context->style.menu_button.normal = background_item;
        m_Context->style.menu_button.hover = background_item;
        m_Context->style.menu_button.active = background_item;
        m_Context->style.menu_button.border_color = border;
        m_Context->style.menu_button.text_background = background;
        m_Context->style.menu_button.text_normal = content;
        m_Context->style.menu_button.text_hover = content;
        m_Context->style.menu_button.text_active = content;

        m_Context->style.tab.tab_minimize_button.normal = inactive_item;
        m_Context->style.tab.tab_minimize_button.hover = inactive_item;
        m_Context->style.tab.tab_minimize_button.active = active_item;

        m_Context->style.tab.text = content;
        m_Context->style.tab.tab_minimize_button.text_background = background;
        m_Context->style.tab.tab_minimize_button.text_normal = content;
        m_Context->style.tab.tab_minimize_button.text_hover = content;
        m_Context->style.tab.tab_minimize_button.text_active = content;

        m_Context->style.tab.tab_maximize_button.normal = inactive_item;
        m_Context->style.tab.tab_maximize_button.hover = inactive_item;
        m_Context->style.tab.tab_maximize_button.active = active_item;
        m_Context->style.tab.tab_maximize_button.text_background = background;
        m_Context->style.tab.tab_maximize_button.text_normal = content;
        m_Context->style.tab.tab_maximize_button.text_hover = content;
        m_Context->style.tab.tab_maximize_button.text_active = content;

        m_Context->style.tab.node_minimize_button.normal = inactive_item;
        m_Context->style.tab.node_minimize_button.hover = inactive_item;
        m_Context->style.tab.node_minimize_button.active = active_item;
        m_Context->style.tab.node_minimize_button.text_background = background;
        m_Context->style.tab.node_minimize_button.text_normal = content;
        m_Context->style.tab.node_minimize_button.text_hover = content;
        m_Context->style.tab.node_minimize_button.text_active = content;

        m_Context->style.tab.node_maximize_button.normal = inactive_item;
        m_Context->style.tab.node_maximize_button.hover = inactive_item;
        m_Context->style.tab.node_maximize_button.active = active_item;
        m_Context->style.tab.node_maximize_button.text_background = background;
        m_Context->style.tab.node_maximize_button.text_normal = content;
        m_Context->style.tab.node_maximize_button.text_hover = content;
        m_Context->style.tab.node_maximize_button.text_active = content;

        m_Context->style.selectable.normal = inactive_item;
        m_Context->style.selectable.hover = inactive_item;
        m_Context->style.selectable.pressed = inactive_item;
        m_Context->style.selectable.normal_active = active_item;
        m_Context->style.selectable.hover_active = active_item;
        m_Context->style.selectable.pressed_active = active_item;
        m_Context->style.selectable.text_normal = content;
        m_Context->style.selectable.text_hover = content;
        m_Context->style.selectable.text_pressed = content;
        m_Context->style.selectable.text_normal_active = content;
        m_Context->style.selectable.text_hover_active = content;
        m_Context->style.selectable.text_pressed_active = content;

        // TODO: Cursor border?
        m_Context->style.slider.normal = background_item;
        m_Context->style.slider.hover = background_item;
        m_Context->style.slider.active = background_item;
        m_Context->style.slider.bar_normal = content;
        m_Context->style.slider.bar_hover = content;
        m_Context->style.slider.bar_active = content;
        m_Context->style.slider.bar_filled = content;
        m_Context->style.slider.cursor_normal = element_item;
        m_Context->style.slider.cursor_hover = element_item;
        m_Context->style.slider.cursor_active = element_item;
        m_Context->style.slider.cursor_size = nk_vec2(16.0f, 16.0f);
        m_Context->style.slider.bar_height = 1.0f;

        m_Context->style.progress.normal = background_item;
        m_Context->style.progress.hover = background_item;
        m_Context->style.progress.active = background_item;
        m_Context->style.progress.cursor_normal = element_item;
        m_Context->style.progress.cursor_hover = element_item;
        m_Context->style.progress.cursor_active = element_item;
        m_Context->style.progress.border_color = border;
        m_Context->style.progress.cursor_border = 0.0f;
        m_Context->style.progress.padding = nk_vec2(0.0f, 0.0f);
        m_Context->style.progress.border = 1.0f;

        m_Context->style.combo.normal = background_item;
        m_Context->style.combo.hover = background_item;
        m_Context->style.combo.active = background_item;
        m_Context->style.combo.border_color = border;
        m_Context->style.combo.label_normal = content;
        m_Context->style.combo.label_hover = content;
        m_Context->style.combo.label_active = content;
        m_Context->style.combo.border = 1.0f;
        m_Context->style.combo.rounding = 0.0f;

        m_Context->style.combo.button.normal = background_item;
        m_Context->style.combo.button.hover = background_item;
        m_Context->style.combo.button.active = background_item;
        m_Context->style.combo.button.text_background = background;
        m_Context->style.combo.button.text_normal = content;
        m_Context->style.combo.button.text_hover = content;
        m_Context->style.combo.button.text_active = content;

        m_Context->style.edit.normal = element_item;
        m_Context->style.edit.hover = element_item;
        m_Context->style.edit.active = element_item;
        m_Context->style.edit.border_color = border;
        m_Context->style.edit.cursor_normal = content;
        m_Context->style.edit.cursor_hover = content;
        m_Context->style.edit.cursor_text_normal = content;
        m_Context->style.edit.cursor_text_hover = content;
        m_Context->style.edit.text_normal = content;
        m_Context->style.edit.text_hover = content;
        m_Context->style.edit.text_active = content;
        m_Context->style.edit.selected_normal = content;
        m_Context->style.edit.selected_hover = content;
        m_Context->style.edit.selected_text_normal = active;
        m_Context->style.edit.selected_text_hover = active;
        m_Context->style.edit.border = 1.0f;

        m_Context->style.property.normal = background_item;
        m_Context->style.property.hover = background_item;
        m_Context->style.property.active = background_item;
        m_Context->style.property.border_color = border;
        m_Context->style.property.label_normal = content;
        m_Context->style.property.label_hover = content;
        m_Context->style.property.label_active = content;
        m_Context->style.property.sym_left = NK_SYMBOL_NONE;
        m_Context->style.property.sym_right = NK_SYMBOL_NONE;
        m_Context->style.property.inc_button = m_Context->style.button;
        m_Context->style.property.dec_button = m_Context->style.button;
        m_Context->style.property.border = 1.0f;

        m_Context->style.property.dec_button.normal = background_item;
        m_Context->style.property.dec_button.hover = background_item;
        m_Context->style.property.dec_button.active = background_item;
        m_Context->style.property.dec_button.text_background = background;
        m_Context->style.property.dec_button.text_normal = content;
        m_Context->style.property.dec_button.text_hover = content;
        m_Context->style.property.dec_button.text_active = content;
        m_Context->style.property.inc_button = m_Context->style.property.dec_button;

        m_Context->style.property.edit.normal = background_item;
        m_Context->style.property.edit.hover = background_item;
        m_Context->style.property.edit.active = background_item;
        m_Context->style.property.edit.border = 0.0f;
        m_Context->style.property.edit.cursor_normal = content;
        m_Context->style.property.edit.cursor_hover = content;
        m_Context->style.property.edit.cursor_text_normal = content;
        m_Context->style.property.edit.cursor_text_hover = content;
        m_Context->style.property.edit.text_normal = content;
        m_Context->style.property.edit.text_hover = content;
        m_Context->style.property.edit.text_active = content;
        m_Context->style.property.edit.selected_normal = content;
        m_Context->style.property.edit.selected_hover = content;
        m_Context->style.property.edit.selected_text_normal = active;
        m_Context->style.property.edit.selected_text_hover = active;

        m_Context->style.chart.background = background_item;
        m_Context->style.chart.border_color = border;
        m_Context->style.chart.color = content;
        m_Context->style.chart.selected_color = active;
        m_Context->style.chart.border = 1.0f;

    }

    void UI::Begin() {

        nk_glfw3_new_frame(&m_NuklearGLFW);

    }
    void UI::End() {

        // Values from the demo backend `main.c`
        nk_glfw3_render(&m_NuklearGLFW, NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

        // TODO: Manage with pipeline object
        //       `Pipeline.Invalidate()`?
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

    }

    void UI::SetBackground(bool background) {

        if(!background) {

            m_Context->style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
            m_Context->style.window.background = nk_rgba(0, 0, 0, 0);

        }
        else {

            SetStyle(m_Style);

        }

    }

    UI::UI(Window& window) {

        // TODO: Switch to `NK_GLFW3_DEFAULT` and deliver events ourselves
        m_Context = nk_glfw3_init(&m_NuklearGLFW, window.m_NativeWindow, NK_GLFW3_INSTALL_CALLBACKS);
        {

            nk_font_atlas* atlas;
            nk_glfw3_font_stash_begin(&m_NuklearGLFW, &atlas);
            nk_font* proggy_clean = nk_font_atlas_add_default(atlas, 14, nullptr);
            nk_glfw3_font_stash_end(&m_NuklearGLFW);
            nk_style_set_font(m_Context, &proggy_clean->handle);

        }

    }

    UI::~UI() {

        nk_glfw3_shutdown(&m_NuklearGLFW);

    }

    UI::operator nk_context*() {

        return m_Context;

    }

}
