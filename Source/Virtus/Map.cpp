namespace Virtus {

    Map::Map(const std::string& path, ResourceLoaderBundle& bundle) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        YAML::Node map = YAML::LoadFile(path);

        for(const auto& element : map["geometry"]) {

            auto mesh_path = element["path"].as<std::string>();
            m_Geometry.emplace_back(bundle.m_MeshLoader.get().Get(mesh_path, bundle));

            glm::vec3 position(
                        element["transform"][0][0].as<float>(),
                        element["transform"][0][1].as<float>(),
                        element["transform"][0][2].as<float>());
            glm::vec3 rotation(
                        glm::radians(element["transform"][1][0].as<float>()),
                        glm::radians(element["transform"][1][1].as<float>()),
                        glm::radians(element["transform"][1][2].as<float>()));
            glm::vec3 scale(
                        element["transform"][2][0].as<float>(),
                        element["transform"][2][1].as<float>(),
                        element["transform"][2][2].as<float>());

            Graphics::Transform transform{position, rotation, scale};
            std::vector<Graphics::Transform> instance {

                transform

            };

            auto material_path = element["material"].as<std::string>();
            Graphics::Material& material = bundle.m_MaterialLoader.get().Get(material_path, bundle);
            std::vector<Graphics::Material::Node> node {

                    material.m_Node

            };
            m_Materials.emplace_back(material);

            for(auto& submesh : m_Geometry.back().get().m_Elements) {

                // TODO: Reusing VBOs with multiple VAOs for materials - `AttachVBO`?
                submesh.m_VAO.CreateVBO(instance, Graphics::BufferUsage::StaticDraw, Graphics::Transform::Layout);
                submesh.m_VAO.CreateVBO(node, Graphics::BufferUsage::StaticDraw, Graphics::Material::Node::Layout);

                Debug(fmt::format("Loaded mesh with {} indices and {} vertices", submesh.m_IndexCount, submesh.m_VertexCount));

            }

        }

        for(const auto& element : map["lights"]["point"]) {

            glm::vec3 position(
                        element["position"][0].as<float>(),
                        element["position"][1].as<float>(),
                        element["position"][2].as<float>());

            glm::vec3 color(
                        element["color"][0].as<float>(),
                        element["color"][1].as<float>(),
                        element["color"][2].as<float>());

            m_PointLightPositions.push_back(position);
            m_PointLightColors.push_back(color);

        }

    }

    void Map::Draw(const Graphics& graphics, Graphics::Shader& shader) {

        std::string u_PointLightCount("u_PointLightCount");
        uint light_count = m_PointLightPositions.size();
        shader.Uniform(u_PointLightCount, light_count);

        std::string u_PointLightPositions("u_PointLightPositions");
        shader.Uniform(u_PointLightPositions, m_PointLightPositions);

        std::string u_PointLightColors("u_PointLightColors");
        shader.Uniform(u_PointLightColors, m_PointLightColors);

        for(usz i = 0; i < m_Geometry.size(); ++i) {

            if(m_Materials[i].get().m_Texture) {

                int unit(0);
                m_Materials[i].get().m_Texture.value().Bind(unit);
                std::string u_Texture("u_Texture");
                shader.Uniform(u_Texture, unit);

            }

            for(auto& element : m_Geometry[i].get().m_Elements) {

                element.Bind();
                graphics.Draw(element.m_IndexCount, 1, Graphics::DrawMode::Indexed);

            }

        }

    }

}
