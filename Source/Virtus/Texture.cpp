namespace Virtus {
    
    static uint FilterModeToGL(Graphics::Texture::FilterMode mode) {

        switch(mode) {

            case Graphics::Texture::FilterMode::Linear: return GL_LINEAR;
            case Graphics::Texture::FilterMode::Nearest: return GL_NEAREST;

        }

    }

    static uint WrapModeToGL(Graphics::Texture::WrapMode mode) {

        switch(mode) {

            case Graphics::Texture::WrapMode::Clamp: return GL_CLAMP_TO_BORDER;
            case Graphics::Texture::WrapMode::Repeat: return GL_REPEAT;

        }

    }
    
    Graphics::Texture::Texture(const Image& image, Graphics::Texture::FilterMode filter, Graphics::Texture::WrapMode wrap) {

        uint handle = 0;
        glGenTextures(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created Texture {}", handle));

        Bind();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterModeToGL(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterModeToGL(filter));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapModeToGL(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapModeToGL(wrap));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) image.m_Width, (int) image.m_Height, 0, image.GetGLFormat(), GL_UNSIGNED_BYTE, image.m_Data.data());

    }

    void Graphics::Texture::Bind(const uint slot) const {

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Handle.get());

    }

    void Graphics::Texture::Bind() const {

        Bind(0);

    }

};
