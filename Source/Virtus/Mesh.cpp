namespace Virtus {

    void Graphics::Mesh::Element::Bind() const {

        m_VAO.Bind();
        m_IBO.Bind();

    }

    Graphics::Mesh::Mesh(const std::string& path, ResourceLoaderBundle& bundle) {

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
                                        aiProcess_Triangulate
                                        | aiProcess_JoinIdenticalVertices 
                                        | aiProcess_SortByPType);

        if(!scene) {

            Fatal(fmt::format("Failed to load `{}`: {}", path, importer.GetErrorString()));

        }

        for(usz i = 0; i < scene->mNumMeshes; ++i) {

            const aiMesh* mesh = scene->mMeshes[i];

            std::vector<Virtus::Graphics::Vertex> vertices;

            for(usz i = 0; i < mesh->mNumVertices; ++i) {

                glm::vec3 position{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                glm::vec4 color{mesh->mColors[0] ?
                    glm::vec4{mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a} :
                    glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
                };
                glm::vec2 UV{mesh->mTextureCoords[0] ?
                        glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y} :
                        glm::vec2{0.0f, 0.0f}};
                glm::vec3 normal{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

                vertices.push_back({position, color, UV, normal});

            }

            std::vector<uint> indices;

            for(usz i = 0; i < mesh->mNumFaces; ++i) {

                usz old = indices.size();
                indices.resize(indices.size() + mesh->mFaces[i].mNumIndices);
                std::memcpy(
                        &indices.data()[old],
                        mesh->mFaces[i].mIndices,
                        mesh->mFaces[i].mNumIndices * sizeof(uint));

            }

            m_Elements.push_back(Graphics::Mesh::Element{{}, {indices, Graphics::BufferUsage::StaticDraw}});

            Graphics::Mesh::Element& element = m_Elements.back();

            element.m_IndexCount = indices.size();
            element.m_VertexCount = vertices.size();

            element.m_VAO.CreateVBO(vertices, Graphics::BufferUsage::StaticDraw, Graphics::Vertex::Layout);

        }

    }

};
