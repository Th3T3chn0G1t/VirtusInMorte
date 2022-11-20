#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    static std::string GLErrorName(uint error) {

        switch(error) {

            case GL_NO_ERROR: return "GL_NO_ERROR";
            case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
            case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
            case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
            case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
            case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";

        }

        return "";

    }

    static void GladPostCallback(void* ret, const char* name, GLADapiproc proc, int n_args, ...) {

        uint error_code = 0;
        while((error_code = glad_glGetError()) != GL_NO_ERROR) {
            Fatal(fmt::format("OpenGL Error {} ({}) in `{}()`", GLErrorName(error_code), error_code, name));
        }

    }

    Graphics::Graphics(Window& window) {

        window.MakeActiveSurface();

        gladSetGLPostCallback(GladPostCallback);

        Info(fmt::format("OpenGL {} {} from {} with GLSL {}",
            (const char*) glGetString(GL_VERSION),
            (const char*) glGetString(GL_RENDERER),
            (const char*) glGetString(GL_VENDOR),
            (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION))); // Kind of annoying that we need the casts but
                                                                      // without them fmt has an aneurism

        int num_extensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

        m_SupportedExtensions.reserve(num_extensions);

        Info(fmt::format("Supported Extensions: {}", num_extensions));
        for(uint i = 0; i < num_extensions; ++i) {

            const char* name = (const char*) glGetStringi(GL_EXTENSIONS, i);

            Info(fmt::format("\t{}", name));

            m_SupportedExtensions.push_back(std::string(name));

        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
        glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
        glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_FASTEST);

    }

    void Graphics::Draw(uint vertices, uint instances, Graphics::DrawMode mode) {

        switch(mode) {

            case Graphics::DrawMode::Normal: glDrawArraysInstanced(GL_TRIANGLES, 0, vertices, instances); break;
            case Graphics::DrawMode::Indexed: glDrawElementsInstanced(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, nullptr, instances); break;

        }

    }

    void Graphics::Clear(glm::vec3 color) {

        glClearColor(color.r, color.g, color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void Graphics::UpdateSurface(Window::Extent extent) {

        glViewport(0, 0, extent[0], extent[1]);

    }

}
