#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    Map::Map(std::string& path, ImageLoader& image_loader, ShaderUnitLoader& shader_unit_loader, MeshLoader& mesh_loader, MaterialLoader& material_loader) {

#ifndef __INTELLISENSE
        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));
#endif

        YAML::Node map = YAML::LoadFile(path);

        for(const auto& element : map["geometry"]) {

            std::string mesh_path = element["path"].as<std::string>();
            m_Geometry.push_back(&mesh_loader.Get(mesh_path));

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

            std::vector<Graphics::Transform> instance {

                {position, rotation, scale}

            };

            for(auto& submesh : m_Geometry.back()->m_Elements) {

                // TODO: Reusing VBOs with multiple VAOs for materials - `AttachVBO`?
                std::string material_path = element["material"].as<std::string>();
                std::vector<Graphics::Material> material {
                    
                    material_loader.Get(material_path)

                };
                submesh.m_VAO.CreateVBO(instance, Graphics::BufferUsage::StaticDraw, Graphics::Transform::Layout);
                submesh.m_VAO.CreateVBO(material, Graphics::BufferUsage::StaticDraw, Graphics::Material::Layout);

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

    void Map::Draw(Graphics& graphics, Graphics::Shader& shader) {

        std::string u_PointLightCount("u_PointLightCount");
        uint light_count = m_PointLightPositions.size();
        shader.Uniform(u_PointLightCount, light_count);

        std::string u_PointLightPositions("u_PointLightPositions");
        shader.Uniform(u_PointLightPositions, m_PointLightPositions);

        std::string u_PointLightColors("u_PointLightColors");
        shader.Uniform(u_PointLightColors, m_PointLightColors);

        for(auto& mesh : m_Geometry) {

            for(auto& element : mesh->m_Elements) {

                element.Bind();
                graphics.Draw(element.m_IndexCount, 1, Graphics::DrawMode::Indexed);

            }

        }

    }

};
