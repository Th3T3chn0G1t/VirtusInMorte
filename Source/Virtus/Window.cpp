namespace Virtus {

    static bool glfw_initialized = false;
    static uint window_count = 0;
    static void GLFWErrorCallback(int error_code, const char* description) {

        Fatal(fmt::format("GLFW {} {}", error_code, description));

    }

    Window::Window(Extent extent, Position position, const std::string& title) {

        if(!glfw_initialized) {
            
            glfwSetErrorCallback(GLFWErrorCallback);
            if(glfwInit() == GLFW_FALSE) {

                Fatal("Failed to initialize GLFW");

            }

            glfw_initialized = true;

        }

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_RESIZABLE, false);

        m_NativeWindow = glfwCreateWindow(extent[0], extent[1], title.c_str(), nullptr, nullptr);

        glfwSetWindowPos(m_NativeWindow, position[0], position[1]);

        window_count++;

    }

    Window::~Window() {

        if(glfw_initialized) { // Apparently we can hit this state since `std::abort` causes cleanup

            glfwDestroyWindow(m_NativeWindow);

            window_count--;
            if(!window_count) {

                glfwTerminate();
                glfw_initialized = false;

            }

        }

    }

    void Window::MakeActiveSurface() {

        glfwMakeContextCurrent(m_NativeWindow);
        gladLoadGL((GLADloadfunc) glfwGetProcAddress);
        
    }

    bool Window::Poll() {

        glfwPollEvents();
        glfwSwapBuffers(m_NativeWindow);

        return glfwWindowShouldClose(m_NativeWindow);

    }

    void Window::SetCursorCapture(bool capture) {

        glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    }

    glm::vec2 Window::GetCursor() const {

        double x = 0.0f;
        double y = 0.0f;
        glfwGetCursorPos(m_NativeWindow, &x, &y);

        return {x, y};

    }

    bool Window::GetKey(int key) const {

        return glfwGetKey(m_NativeWindow, key);

    }

}
