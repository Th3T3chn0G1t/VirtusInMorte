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

}
