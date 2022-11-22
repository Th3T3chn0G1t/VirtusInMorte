#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    Graphics::BufferLayout Graphics::Vertex::Layout = {

        {VertexAttribute::Type::FullFloat, 3, 0},
        {VertexAttribute::Type::FullFloat, 4, 0},
        {VertexAttribute::Type::FullFloat, 2, 0},
        {VertexAttribute::Type::FullFloat, 3, 0}

    };

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

}
