#pragma once

#include <Common.hpp>

#include <stb_image.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <nuklear.h>
extern "C" {
    #include <nuklear_glfw_gl3.h>
}

namespace Virtus {

    using namespace Common;

    class UI;

    class Window {

    public:
        using Extent = uint[2];
        using Position = int[2];

    private:
        GLFWwindow* m_NativeWindow;

    public:
        Window(Extent, Position, const std::string&);
        ~Window();

        void MakeActiveSurface();
        bool Poll();
        void SetCursorCapture(bool);
        glm::vec2 GetCursor() const;
        bool GetKey(int) const;

        friend class UI;

    };

    class UI {

    public:
        class Style {

        public:
            glm::uvec4 m_Border;
            glm::uvec4 m_Background;
            glm::uvec4 m_Content;
            glm::uvec4 m_Inactive;
            glm::uvec4 m_Element;
            glm::uvec4 m_Active;

        public:
            Style() = default;

            Style(glm::vec4 border, glm::vec4 background, glm::vec4 content, glm::vec4 inactive, glm::vec4 element, glm::vec4 active) :
                m_Border(border),
                m_Background(background),
                m_Content(content),
                m_Inactive(inactive),
                m_Element(element),
                m_Active(active) {}

            explicit Style(std::string&);

        };

    private:
        nk_glfw m_NuklearGLFW;
        nk_context* m_Context;

    public:
        Style m_Style;

    public:
        explicit UI(Window&);
        ~UI();

        void Begin();
        void End();
        void SetStyle(Style&);
        void SetBackground(bool);

        operator nk_context*();

    };

    class Graphics {

    public:
        class Texture;
        class Image {

            public:
                enum class Format {

                    R8,
                    RG8,
                    RGB8,
                    RGBA8

                };

            private:
                usz m_Width;
                usz m_Height;
                Format m_Format;
                std::vector<uchar> m_Data;

            private:
                uint GetGLFormat() const;

            public:
                Image() = default;
                explicit Image(std::string&);

                friend class Texture;

        };

        class GLHandle {

        public:
            uint m_UnderlyingHandle{0};

        public:
            GLHandle() = default;
            GLHandle(const std::nullptr_t) {};
            GLHandle(const uint handle) : m_UnderlyingHandle(handle) {}

            operator uint() const { return m_UnderlyingHandle; }

            explicit operator bool() const { return m_UnderlyingHandle != 0; }
            bool operator!=(const std::nullptr_t) const { return m_UnderlyingHandle != 0; }

        };

        template<void (*func)(const uint)>
        class GLDestructor {

        public:
            using pointer = GLHandle;

        public:
            void operator()(const GLHandle handle) const {
                func(handle);
            }

        };

        class Texture {

        private:
            static void Deleter(const uint handle) { Debug(fmt::format("Deleting Texture {}", handle)); glDeleteTextures(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;        

        public:
            enum class FilterMode {

                Linear,
                Nearest

            };

            enum class WrapMode {

                Clamp,
                Repeat

            };

        private:
            Handle m_Handle;

        public:
            Texture(const Image&, const FilterMode, const WrapMode);

            void Bind(const uint) const;
            void Bind() const;

        };

        class Shader;
        class Shader {

        private:
            static void Deleter(const uint handle) { Debug(fmt::format("Deleting Shader {}", handle)); glDeleteProgram(handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        public:
            class Unit {

            private:
                static void Deleter(const uint handle) { Debug(fmt::format("Deleting Shader Unit {}", handle)); glDeleteShader(handle); };
                using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

            public:
                enum class Stage {

                    Vertex,
                    Fragment

                };

            private:
                Stage m_Stage;
                Handle m_Handle;

            public:
                Unit() = default;
                explicit Unit(const std::string&);

                friend class Shader;
    
            };

        private:
            Handle m_Handle;
            std::unordered_map<std::string, uint> m_Uniforms;

        public:
            Shader(const Graphics::Shader::Unit&, const Graphics::Shader::Unit&);

            void Bind() const;

            template<class T>
            void Uniform(const std::string& name, const T& value) {

                auto emplaced = m_Uniforms.try_emplace(name, -1);
                auto it = emplaced.first;
                auto added = emplaced.second;

                if(added) it->second = glGetUniformLocation(m_Handle.get(), name.c_str());

                if constexpr (std::is_same<T, float    >::value) glUniform1f(it->second, value);
                else if constexpr (std::is_same<T, glm::vec2>::value) glUniform2f(it->second, value.x, value.y);
                else if constexpr (std::is_same<T, glm::vec3>::value) glUniform3f(it->second, value.x, value.y, value.z);
                else if constexpr (std::is_same<T, glm::vec4>::value) glUniform4f(it->second, value.x, value.y, value.z, value.w);

                else if constexpr (std::is_same<T, int       >::value) glUniform1i(it->second, value);
                else if constexpr (std::is_same<T, glm::ivec2>::value) glUniform2i(it->second, value.x, value.y);
                else if constexpr (std::is_same<T, glm::ivec3>::value) glUniform3i(it->second, value.x, value.y, value.z);
                else if constexpr (std::is_same<T, glm::ivec4>::value) glUniform4i(it->second, value.x, value.y, value.z, value.w);

                else if constexpr (std::is_same<T, uint      >::value) glUniform1ui(it->second, value);
                else if constexpr (std::is_same<T, glm::uvec2>::value) glUniform2ui(it->second, value.x, value.y);
                else if constexpr (std::is_same<T, glm::uvec3>::value) glUniform3ui(it->second, value.x, value.y, value.z);
                else if constexpr (std::is_same<T, glm::uvec4>::value) glUniform4ui(it->second, value.x, value.y, value.z, value.w);

                else if constexpr (std::is_same<T, glm::mat2x2>::value) glUniformMatrix2fv(it->second, 1, false, (float*) &value);
                else if constexpr (std::is_same<T, glm::mat3x3>::value) glUniformMatrix3fv(it->second, 1, false, (float*) &value);
                else if constexpr (std::is_same<T, glm::mat4x4>::value) glUniformMatrix4fv(it->second, 1, false, (float*) &value);

                else if constexpr (std::is_same<T, std::vector<float    >>::value) glUniform1fv(it->second, value.size(), value.data());
                else if constexpr (std::is_same<T, std::vector<glm::vec2>>::value) glUniform2fv(it->second, value.size(), (float*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::vec3>>::value) glUniform3fv(it->second, value.size(), (float*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::vec4>>::value) glUniform4fv(it->second, value.size(), (float*) value.data());

                else if constexpr (std::is_same<T, std::vector<int       >>::value) glUniform1iv(it->second, value.size(), value.data());
                else if constexpr (std::is_same<T, std::vector<glm::ivec2>>::value) glUniform2iv(it->second, value.size(), (int*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::ivec3>>::value) glUniform3iv(it->second, value.size(), (int*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::ivec4>>::value) glUniform4iv(it->second, value.size(), (int*) value.data());

                else if constexpr (std::is_same<T, std::vector<uint      >>::value) glUniform1uiv(it->second, value.size(), value.data());
                else if constexpr (std::is_same<T, std::vector<glm::uvec2>>::value) glUniform2uiv(it->second, value.size(), (uint*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::uvec3>>::value) glUniform3uiv(it->second, value.size(), (uint*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::uvec4>>::value) glUniform4uiv(it->second, value.size(), (uint*) value.data());

                else if constexpr (std::is_same<T, std::vector<glm::mat2x2>>::value) glUniformMatrix2fv(it->second, value.size(), false, (float*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::mat3x3>>::value) glUniformMatrix3fv(it->second, value.size(), false, (float*) value.data());
                else if constexpr (std::is_same<T, std::vector<glm::mat4x4>>::value) glUniformMatrix4fv(it->second, value.size(), false, (float*) value.data());

                else static_assert("Not a valid uniform type");

            }

        };

        enum class DrawMode {

            Normal,
            Indexed

        };

        enum class BufferUsage {

            StaticRead,
            StaticDraw,

            DynamicRead,
            DynamicDraw

        };

        class VertexAttribute {

        public:
            enum class Type {

                SignedByte,
                UnsignedByte,

                SignedShort,
                UnsignedShort,

                SignedInt,
                UnsignedInt,

                HalfFloat,
                FullFloat,
                DoubleFloat

            };

        public:
            Type m_Type;
            uint m_Count;
            uint m_Divisor;

        public:
            VertexAttribute(const Type type, const uint count, const uint divisor) : m_Type(type), m_Count(count), m_Divisor(divisor) {}

        };

        using BufferLayout = std::vector<VertexAttribute>;

        class Vertex {

        public:
            static BufferLayout Layout;

        public:
            glm::vec3 m_Position;
            glm::vec4 m_Color;
            glm::vec2 m_UV;
            glm::vec3 m_Normal;

        };

        class Transform {

        public:
            static BufferLayout Layout;

        public:
            glm::mat4 m_Transform;

        private:
            glm::mat3 m_Normal;

        public:
            explicit Transform(const glm::vec3& position = {0.0f, 0.0f, 0.0f}, const glm::vec3& rotation = {0.0f, 0.0f, 0.0f}, const glm::vec3& scale = {1.0f, 1.0f, 1.0f});

        };

        class Material {

        public:
            static BufferLayout Layout;

        public:
            uint m_SpecularStrength;
            float m_Shininess;
            uint m_DoSample;

        public:
            Material() = default;
            Material(const uint specular_strength, const float shininess, const uint do_sample) : m_SpecularStrength(specular_strength), m_Shininess(shininess), m_DoSample(do_sample) {}
            explicit Material(const std::string&);

        };

        class VAO;

        class VBO {

        private:
            static void Deleter(const uint handle) { Debug(fmt::format("Deleting VBO {}", handle)); glDeleteBuffers(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        private:
            Handle m_Handle;

        private:
            VBO(const void* data, uint size, Graphics::BufferUsage usage);

        public:
            void Bind() const;

            friend class VAO;

        };

        class VAO {

        private:
            static void Deleter(uint handle) { Debug(fmt::format("Deleting VAO {}", handle)); glDeleteVertexArrays(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        private:
            Handle m_Handle;

            std::vector<VBO> m_VBOs;

            uint m_AttributeIndex{0};

        private:
            void ApplyLayout(const BufferLayout&);
            VBO& CreateVBO(const void*, uint, BufferUsage, const BufferLayout&);

        public:
            VAO();

            void Bind() const;

            template<class T>
            VBO& CreateVBO(const std::vector<T> data, const Graphics::BufferUsage usage, const Graphics::BufferLayout& layout) {

                return CreateVBO((void*) data.data(), data.size() * sizeof(T), usage, layout);

            }

        };

        class IBO {

        private:
            static void Deleter(const uint handle) { Debug(fmt::format("Deleting IBO {}", handle)); glDeleteBuffers(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        private:
            Handle m_Handle;

        public:
            IBO(const std::vector<uint>&, BufferUsage);

            void Bind() const;

        };

        class Mesh {

        public:
            class Element {

            public:
                VAO m_VAO;
                IBO m_IBO;

                usz m_IndexCount;
                usz m_VertexCount;

            public:
                void Bind() const;

            };

        public:
            std::vector<Element> m_Elements;

        public:
            Mesh() = default;
            explicit Mesh(const std::string&);

        };

    private:
        std::vector<std::string> m_SupportedExtensions;

    public:
        explicit Graphics(Window&);
        Graphics() = default;

        void Draw(uint, uint, DrawMode) const;
        void Clear(const glm::vec3&);

    };

    using ImageLoader = ResourceLoader<Graphics::Image, 1024>;
    using ShaderUnitLoader = ResourceLoader<Graphics::Shader::Unit, 1024>;
    using MeshLoader = ResourceLoader<Graphics::Mesh, 1024>;
    using MaterialLoader = ResourceLoader<Graphics::Material, 1024>;

    using UIStyleLoader = ResourceLoader<UI::Style, 128>;

    class Map {

    public:
        std::vector<Graphics::Mesh*> m_Geometry;
        std::vector<glm::vec3> m_PointLightPositions;
        std::vector<glm::vec3> m_PointLightColors;

    public:
        Map(const std::string&, ImageLoader&, ShaderUnitLoader&, MeshLoader&, MaterialLoader&);

        void Draw(const Graphics&, Graphics::Shader&);

    };

}
