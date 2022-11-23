#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    Graphics::BufferLayout Graphics::Transform::Layout = {

        // TODO: Special case for matrices
        {VertexAttribute::Type::FullFloat, 4, 1},
        {VertexAttribute::Type::FullFloat, 4, 1},
        {VertexAttribute::Type::FullFloat, 4, 1},
        {VertexAttribute::Type::FullFloat, 4, 1},

        {VertexAttribute::Type::FullFloat, 3, 1},
        {VertexAttribute::Type::FullFloat, 3, 1},
        {VertexAttribute::Type::FullFloat, 3, 1}

    };

    Graphics::Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
        transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
        transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, scale);

        m_Transform = transform;
        m_Normal = glm::mat3(glm::transpose(glm::inverse(transform)));

    }

}
