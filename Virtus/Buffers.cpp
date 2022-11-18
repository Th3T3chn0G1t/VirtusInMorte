#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Virtus {

    static uint UsageToGL(Graphics::BufferUsage usage) {

        switch(usage) {

            case Graphics::BufferUsage::StaticRead: return GL_STATIC_READ;
            case Graphics::BufferUsage::StaticDraw: return GL_STATIC_DRAW;
            case Graphics::BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
            case Graphics::BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;

        }

    }
    
    void Graphics::BufferLayout::Push(Member member) {
    
        m_Layout.push_back(member);
    
    }

    Graphics::VAO::VAO(BufferLayout layout) : m_Layout(layout) {
        
        // We can't setup buffer layout here since there is no vertex buffer bound
        // so creation of a vertex buffer is liased through the VAO and we can defer
        // buffer layout until then
        uint handle = 0;
        glGenVertexArrays(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created VAO {}", handle));

    }

    void Graphics::VAO::ApplyLayout() {

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
        details.reserve(m_Layout.m_Layout.size());

        uint total_size = 0;

        for(auto& member : m_Layout.m_Layout) {
            
            AttributeDetail detail;

            switch(member.m_Type) {

                case Graphics::BufferLayout::Member::Type::SignedByte: detail = {GL_BYTE, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::UnsignedByte: detail = {GL_UNSIGNED_BYTE, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::SignedShort: detail = {GL_SHORT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::UnsignedShort: detail = {GL_UNSIGNED_SHORT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::SignedInt: detail = {GL_INT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::UnsignedInt: detail = {GL_UNSIGNED_INT, AttributeDetail::Base::Integer, 0}; break;
                case Graphics::BufferLayout::Member::Type::HalfFloat: detail = {GL_HALF_FLOAT, AttributeDetail::Base::Float, 0}; break;
                case Graphics::BufferLayout::Member::Type::FullFloat: detail = {GL_FLOAT, AttributeDetail::Base::Float, 0}; break;
                case Graphics::BufferLayout::Member::Type::DoubleFloat: detail = {GL_DOUBLE, AttributeDetail::Base::Double, 0}; break;

            }

            uint type_size = 0;
            switch(detail.m_Type) {

                case GL_BYTE: type_size = sizeof(char); break;
                case GL_UNSIGNED_BYTE: type_size = sizeof(uchar); break;
                case GL_SHORT: type_size = sizeof(short); break;
                case GL_UNSIGNED_SHORT: type_size = sizeof(ushort); break;
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
            auto& member = m_Layout.m_Layout[i];

            switch(detail.m_Base) {

                case AttributeDetail::Base::Integer: glVertexAttribIPointer(i, member.m_Count, detail.m_Type, total_size, (void*) (uptr) offset); break;
                case AttributeDetail::Base::Float: glVertexAttribPointer(i, member.m_Count, detail.m_Type, false, total_size, (void*) (uptr) offset); break;
                case AttributeDetail::Base::Double: glVertexAttribLPointer(i, member.m_Count, detail.m_Type, total_size, (void*) (uptr) offset); break;

            }

            glEnableVertexAttribArray(i);
            glVertexAttribDivisor(i, member.m_Divisor);

            offset += detail.m_Size;

        }

        m_LayoutApplied = true;

    }

    void Graphics::VAO::Bind() {

        glBindVertexArray(m_Handle.get());

    }

    Graphics::VBO& Graphics::VAO::CreateVBO(void* data, uint size, Graphics::BufferUsage usage) {

        Bind();
        m_VBOs.push_back(Graphics::VBO(data, size, usage));

        if(!m_LayoutApplied) ApplyLayout();

        return m_VBOs.back();

    }

    Graphics::VBO::VBO(void* data, uint size, Graphics::BufferUsage usage) {

        uint handle = 0;
        glGenBuffers(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created VBO {}", handle));

        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGL(usage));

    }

    void Graphics::VBO::Bind() {

        glBindBuffer(GL_ARRAY_BUFFER, m_Handle.get());

    }

    Graphics::IBO::IBO(std::vector<uint>& indices, BufferUsage usage) {

        uint handle = 0;
        glGenBuffers(1, &handle);
        m_Handle.reset(handle);

        Debug(fmt::format("Created IBO {}", handle));

        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), UsageToGL(usage));

    }

    void Graphics::IBO::Bind() {
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle.get());
    
    }

}
