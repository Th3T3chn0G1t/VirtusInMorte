namespace Virtus {

    Graphics::BufferLayout Graphics::Material::Node::Layout = {

        {VertexAttribute::Type::UnsignedInt, 1, 1},
        {VertexAttribute::Type::FullFloat, 1, 1},

        {VertexAttribute::Type::UnsignedInt, 1, 1}

    };

    Graphics::Material::Material(const std::string& path, ResourceLoaderBundle& bundle) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        YAML::Node map = YAML::LoadFile(path);

        m_Node = {map["specular"].as<uint>(16), map["shininess"].as<float>(0.5f), true};

        auto texture = map["texture"].as<std::string>("__THIS_IS_NOT_A_TEXTURE");
        if(texture == "__THIS_IS_NOT_A_TEXTURE") {

            m_Node.m_DoSample = false;

        }
        else {

            // TODO: Dedup textures with a resource loader
            m_Texture = Graphics::Texture(bundle.m_ImageLoader.get().Get(texture, bundle), Graphics::Texture::FilterMode::Linear, Graphics::Texture::WrapMode::Clamp);

        }

    }

}

