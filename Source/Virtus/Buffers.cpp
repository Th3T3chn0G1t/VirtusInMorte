namespace Virtus {

    static uint UsageToGL(Graphics::BufferUsage usage) {

        switch(usage) {

            case Graphics::BufferUsage::StaticRead: return GL_STATIC_READ;
            case Graphics::BufferUsage::StaticDraw: return GL_STATIC_DRAW;
            case Graphics::BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
            case Graphics::BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;

        }

    }

    Graphics::VAO::VAO() {
        
        uint handle = 0;
        glGenVertexArrays(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created VAO {}", handle));

    }

    void Graphics::VAO::ApplyLayout(const Graphics::BufferLayout& layout) {

        Debug(fmt::format("Setting attributes on VAO {}", (uint) m_Handle.get()));

        struct AttributeDetail {

            enum class Base {
                
                Integer,
                Float,
                Double

            };

            uint m_Type;
            Base m_Base;
            uint m_Size;

        };

        std::vector<AttributeDetail> details;
        details.reserve(layout.size());

        uint total_size = 0;

        for(auto& member : layout) {
            
            AttributeDetail detail{};

            switch(member.m_Type) {

                case Graphics::VertexAttribute::Type::SignedByte: detail = {GL_BYTE, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::UnsignedByte: detail = {GL_UNSIGNED_BYTE, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::SignedShort: detail = {GL_SHORT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::UnsignedShort: detail = {GL_UNSIGNED_SHORT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::SignedInt: detail = {GL_INT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::UnsignedInt: detail = {GL_UNSIGNED_INT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::VertexAttribute::Type::HalfFloat: detail = {GL_HALF_FLOAT, AttributeDetail::Base::Float, 0}; break;
                case Graphics::VertexAttribute::Type::FullFloat: detail = {GL_FLOAT, AttributeDetail::Base::Float, 0}; break;
                case Graphics::VertexAttribute::Type::DoubleFloat: detail = {GL_DOUBLE, AttributeDetail::Base::Double, 0}; break;

            }

            uint type_size = 0;
            switch(detail.m_Type) {

                case GL_BYTE: type_size = sizeof(char); break;
                case GL_UNSIGNED_BYTE: type_size = sizeof(uchar); break;
                case GL_SHORT: type_size = sizeof(short); break;
                case GL_UNSIGNED_SHORT: type_size = sizeof(Common::ushort); break;
                case GL_INT: type_size = sizeof(int); break;
                case GL_UNSIGNED_INT: type_size = sizeof(uint); break;
                case GL_HALF_FLOAT: type_size = sizeof(half); break;
                case GL_FLOAT: type_size = sizeof(float); break;
                case GL_DOUBLE: type_size = sizeof(double); break;
            
            }

            detail.m_Size = member.m_Count * type_size;

            total_size += detail.m_Size;

            details.push_back(detail);

        }

        uint offset = 0;
        for(uint i = 0; i < details.size(); ++i) {

            auto& detail = details[i];
            auto& member = layout[i];

            switch(detail.m_Base) {

                case AttributeDetail::Base::Integer: glVertexAttribIPointer(m_AttributeIndex + i, static_cast<int>(member.m_Count), detail.m_Type, static_cast<int>(total_size), (void*) (uptr) offset); break;
                case AttributeDetail::Base::Float: glVertexAttribPointer(m_AttributeIndex + i, static_cast<int>(member.m_Count), detail.m_Type, false, static_cast<int>(total_size), (void*) (uptr) offset); break;
                case AttributeDetail::Base::Double: glVertexAttribLPointer(m_AttributeIndex + i, static_cast<int>(member.m_Count), detail.m_Type, static_cast<int>(total_size), (void*) (uptr) offset); break;

            }

            glEnableVertexAttribArray(m_AttributeIndex + i);
            glVertexAttribDivisor(m_AttributeIndex + i, member.m_Divisor);

            offset += detail.m_Size;

        }

        m_AttributeIndex += details.size();

    }

    void Graphics::VAO::Bind() const {

        glBindVertexArray(m_Handle.get());

    }


    Graphics::VBO& Graphics::VAO::CreateVBO(const void* data, uint size, Graphics::BufferUsage usage, const Graphics::BufferLayout& layout) {

        Bind();
        m_VBOs.push_back(Graphics::VBO(data, size, usage));

        Graphics::VBO& vbo = m_VBOs.back();
        vbo.Bind();
        ApplyLayout(layout);

        return vbo;

    }

    Graphics::VBO::VBO(const void* const data, const uint size, const Graphics::BufferUsage usage) {

        uint handle = 0;
        glGenBuffers(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created VBO {}", handle));

        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGL(usage));

    }

    void Graphics::VBO::Bind() const {

        glBindBuffer(GL_ARRAY_BUFFER, m_Handle.get());

    }

    Graphics::IBO::IBO(const std::vector<uint>& indices, BufferUsage usage) {

        uint handle = 0;
        glGenBuffers(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created IBO {}", handle));

        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices.size() * sizeof(uint)), indices.data(), UsageToGL(usage));

    }

    void Graphics::IBO::Bind() const {
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle.get());
    
    }

}
