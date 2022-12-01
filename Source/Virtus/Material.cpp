namespace Virtus {

    Graphics::BufferLayout Graphics::Material::Layout = {

        {VertexAttribute::Type::UnsignedInt, 1, 1},
        {VertexAttribute::Type::FullFloat, 1, 1},

        {VertexAttribute::Type::UnsignedInt, 1, 1}

    };

    Graphics::Material::Material(const std::string& path, ResourceLoaderBundle& bundle) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        YAML::Node map = YAML::LoadFile(path);

        m_SpecularStrength = map["specular"].as<uint>(16);
        m_Shininess = map["shininess"].as<float>(0.5f);
        m_DoSample = map["sample"].as<bool>(false);

    }

}

