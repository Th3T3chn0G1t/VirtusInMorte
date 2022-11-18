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

        uint error_code = glad_glGetError();

        if(error_code != GL_NO_ERROR) Fatal(fmt::format("OpenGL Error {} ({}) in `{}()`", GLErrorName(error_code), error_code, name));

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

    }

    void Graphics::Draw(uint vertices, uint instances, Graphics::DrawMode mode) {

        switch(mode) {

            case Graphics::DrawMode::Normal: glDrawArraysInstanced(GL_TRIANGLES, 0, vertices, instances);
            case Graphics::DrawMode::Indexed: glDrawElementsInstanced(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, nullptr, instances);

        }

    }

}
