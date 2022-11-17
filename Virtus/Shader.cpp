#include <Virtus.hpp>

namespace Virtus {

    static uint StageToGL(Graphics::Shader::Unit::Stage stage) {

        switch(stage) {

            case Graphics::Shader::Unit::Stage::Vertex: return GL_VERTEX_SHADER;
            case Graphics::Shader::Unit::Stage::Fragment: return GL_FRAGMENT_SHADER;

        }

    }

    Graphics::Shader::Unit::Unit(Graphics::Shader::Unit::Stage stage, std::string& source) : m_Stage(stage) {

        m_Handle = glCreateShader(StageToGL(stage));
        const char* data = source.c_str();
        glShaderSource(m_Handle, 1, &data, nullptr);
        glCompileShader(m_Handle);

        int result = 0;
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result);
        if(!result) {
            int length = 0;
            glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

            char* message = new char[length + 1];
            glGetShaderInfoLog(m_Handle, length, nullptr, message);

            Fatal(fmt::format("Failed to compile shader unit:\n{}", message));
        }

    }

    Graphics::Shader::Unit::~Unit() {

        Fatal(fmt::format("Deleting shader {}", m_Handle));
        glDeleteShader(m_Handle);

    }

    Graphics::Shader::Shader(std::vector<Graphics::Shader::Unit>& units) {

        m_Handle = glCreateProgram();

        for(auto& unit : units) {
            glAttachShader(m_Handle, unit.m_Handle);
        }

        glLinkProgram(m_Handle);        

        int result = 0;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &result);
        if(!result) {
            int length = 0;
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

            char* message = new char[length + 1];
            glGetProgramInfoLog(m_Handle, length, nullptr, message);

            Fatal(fmt::format("Failed to link shader:\n{}", message));
        }

        for(auto& unit : units) {
            glDetachShader(m_Handle, unit.m_Handle);
        }

    }

    Graphics::Shader::~Shader() {

        glDeleteProgram(m_Handle);

    }

    void Graphics::Shader::Bind() {

        glUseProgram(m_Handle);

    }

}
