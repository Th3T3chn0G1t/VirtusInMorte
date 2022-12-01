namespace Virtus {

    uint Graphics::Image::GetGLFormat() const {

        switch(m_Format) {

            case Graphics::Image::Format::R8: return GL_RED;
            case Graphics::Image::Format::RG8: return GL_RG;
            case Graphics::Image::Format::RGB8: return GL_RGB;
            case Graphics::Image::Format::RGBA8: return GL_RGBA;

        }

        return 0;

    }

    Graphics::Image::Image(std::string& path) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(true);
        uchar* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        usz count = width * height * channels;

        m_Data.resize(count);

        std::memcpy(m_Data.data(), data, count * sizeof(uchar));

        m_Width = width;
        m_Height = height;

        switch(channels) {

            case 1: m_Format = Graphics::Image::Format::R8; break;
            case 2: m_Format = Graphics::Image::Format::RG8; break;
            case 3: m_Format = Graphics::Image::Format::RGB8; break;
            case 4: m_Format = Graphics::Image::Format::RGBA8; break;

        }

        stbi_image_free(data);

    }

}
