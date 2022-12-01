namespace Virtus {

    static uint StageToGL(Graphics::Shader::Unit::Stage stage) {

        switch(stage) {

            case Graphics::Shader::Unit::Stage::Vertex: return GL_VERTEX_SHADER;
            case Graphics::Shader::Unit::Stage::Fragment: return GL_FRAGMENT_SHADER;

        }

    }

    Graphics::Shader::Unit::Unit(const std::string& path) {

        std::string source;
        if(std::ifstream stream {path, std::ios::binary | std::ios::ate}) {

            auto size(stream.tellg());
            source.resize(size);
            stream.seekg(0);
            stream.read(source.data(), size);

        }
        else {

            Virtus::Fatal(fmt::format("Failed to load shader unit from `{}`", path));

        }

        Graphics::Shader::Unit::Stage stage;

        if(source.find("#pragma vertex") != std::string::npos) {

            stage = Graphics::Shader::Unit::Stage::Vertex;

        }
        else if(source.find("#pragma fragment") != std::string::npos) {

            stage = Graphics::Shader::Unit::Stage::Fragment;

        }
        else {

            Fatal(fmt::format("No stage specified in shader unit `{}`", path));

        }

        m_Stage = stage;

        m_Handle.reset(glCreateShader(StageToGL(stage)));

        Debug(fmt::format("Created Shader Unit {}", (uint) m_Handle.get()));

        const char* data = source.c_str();
        glShaderSource(m_Handle.get(), 1, &data, nullptr);
        glCompileShader(m_Handle.get());

        int result = 0;
        glGetShaderiv(m_Handle.get(), GL_COMPILE_STATUS, &result);
        if(!result) {
            
            int length = 0;
            glGetShaderiv(m_Handle.get(), GL_INFO_LOG_LENGTH, &length);

            std::string message;
            message.resize(length + 1);
            glGetShaderInfoLog(m_Handle.get(), length, nullptr, message.data());

            Fatal(fmt::format("Failed to compile shader unit:\n{}", message));

        }

    }

    Graphics::Shader::Shader(const Graphics::Shader::Unit& vertex, const Graphics::Shader::Unit& fragment) {

        m_Handle.reset(glCreateProgram());

        Debug(fmt::format("Created Shader {}", (uint) m_Handle.get()));

        glAttachShader(m_Handle.get(), vertex.m_Handle.get());
        glAttachShader(m_Handle.get(), fragment.m_Handle.get());

        glLinkProgram(m_Handle.get());        

        int result = 0;
        glGetProgramiv(m_Handle.get(), GL_LINK_STATUS, &result);
        if(!result) {
            int length = 0;
            glGetProgramiv(m_Handle.get(), GL_INFO_LOG_LENGTH, &length);

            std::string message;
            message.resize(length + 1);
            glGetProgramInfoLog(m_Handle.get(), length, nullptr, message.data());

            Fatal(fmt::format("Failed to link shader:\n{}", message.data()));
        }

        glDetachShader(m_Handle.get(), vertex.m_Handle.get());
        glDetachShader(m_Handle.get(), fragment.m_Handle.get());

    }

    void Graphics::Shader::Bind() const {

        glUseProgram(m_Handle.get());

    }

}
