#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    Graphics::BufferLayout Graphics::Material::Layout = {

        {VertexAttribute::Type::UnsignedInt, 1, 1},
        {VertexAttribute::Type::FullFloat, 1, 1},

        {VertexAttribute::Type::UnsignedInt, 1, 1}

    };

    Graphics::Material::Material(std::string& path) {

#ifndef __INTELLISENSE
        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));
#endif

        YAML::Node map = YAML::LoadFile(path);

        m_SpecularStrength = map["specular"].as<uint>(16);
        m_Shininess = map["shininess"].as<float>(0.5f);
        m_DoSample = map["sample"].as<bool>(false);

    }

}

