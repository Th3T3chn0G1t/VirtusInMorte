#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <functional>
#include <filesystem>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include <stb_image.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <fmt/color.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <yaml-cpp/yaml.h>

namespace Virtus {

    using uchar = unsigned char;
    using ushort = unsigned char;
    using uint = unsigned int;
#ifdef __INTELLISENSE
    using half = float;
#else
    using half = __fp16;
#endif
    using uptr = std::uintptr_t;
    using usz = std::size_t;

    void Fatal(std::string);
    void Error(std::string);
    void Info(std::string);
    void Debug(std::string);

    class Window {

    public:
        using Extent = uint[2];
        using Position = int[2];

    private:
        GLFWwindow* m_NativeWindow;

    public:
        Window(Extent, Position, std::string&);
        ~Window();

        void MakeActiveSurface();
        bool Poll();
        void SetCursorCapture(bool);
        glm::vec2 GetCursor();
        bool GetKey(int);

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
                uint GetGLFormat();

            public:
                Image() = default;
                Image(std::string&);

                friend class Texture;

        };

        class GLHandle {

        public:
            uint m_UnderlyingHandle{0};

        public:
            GLHandle() {};
            GLHandle(std::nullptr_t) {};
            GLHandle(uint handle) : m_UnderlyingHandle(handle) {}

            operator uint() { return m_UnderlyingHandle; }

            explicit operator bool() { return m_UnderlyingHandle != 0; }
            bool operator!=(std::nullptr_t) { return m_UnderlyingHandle != 0; }

        };

        template<void (*func)(uint)>
        class GLDestructor {

        public:
            using pointer = GLHandle;

        public:
            void operator()(GLHandle handle) {
                func(handle);
            }

        };

        class Texture {

        private:
            static void Deleter(uint handle) { Debug(fmt::format("Deleting Texture {}", handle)); glDeleteTextures(1, &handle); };
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
            Texture(Image&, FilterMode, WrapMode);

            void Bind(uint);
            void Bind();

        };

        class Shader;
        class Shader {

        private:
            static void Deleter(uint handle) { Debug(fmt::format("Deleting Shader {}", handle)); glDeleteProgram(handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        public:
            class Unit {

            private:
                static void Deleter(uint handle) { Debug(fmt::format("Deleting Shader Unit {}", handle)); glDeleteShader(handle); };
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
                Unit(std::string&);

                friend class Shader;
    
            };

        private:
            Handle m_Handle;
            std::unordered_map<std::string, uint> m_Uniforms;

        public:
            Shader(Graphics::Shader::Unit&, Graphics::Shader::Unit&);

            void Bind();

            template<class T>
            void Uniform(std::string& name, T& value) {

                auto empl = m_Uniforms.try_emplace(name, -1);
                auto it = empl.first;
                auto added = empl.second;

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
            VertexAttribute(Type type, uint count, uint divisor) : m_Type(type), m_Count(count), m_Divisor(divisor) {}

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
            Transform(glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scale = {1.0f, 1.0f, 1.0f}) {

                glm::mat4 transform(1.0f);

                transform = glm::translate(transform, position);
                transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
                transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
                transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
                transform = glm::scale(transform, scale);

                m_Transform = transform;
                m_Normal = glm::mat3(glm::transpose(glm::inverse(transform)));

            }

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
            Material(uint specular_strength, float shininess, uint do_sample) : m_SpecularStrength(specular_strength), m_Shininess(shininess), m_DoSample(do_sample) {}
            Material(std::string&);

        };

        class VAO;

        class VBO {

        private:
            static void Deleter(uint handle) { Debug(fmt::format("Deleting VBO {}", handle)); glDeleteBuffers(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        private:
            Handle m_Handle;

        private:
            VBO(void* data, uint size, Graphics::BufferUsage usage);

        public:
            void Bind();

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
            void ApplyLayout(BufferLayout&);
            VBO& CreateVBO(void*, uint, BufferUsage, BufferLayout&);

        public:
            VAO();

            void Bind();

            template<class T>
            VBO& CreateVBO(std::vector<T> data, Graphics::BufferUsage usage, Graphics::BufferLayout& layout) {

                return CreateVBO((void*) data.data(), data.size() * sizeof(T), usage, layout);

            }

        };

        class IBO {

        private:
            static void Deleter(uint handle) { Debug(fmt::format("Deleting IBO {}", handle)); glDeleteBuffers(1, &handle); };
            using Handle = std::unique_ptr<GLHandle, GLDestructor<Deleter>>;

        private:
            Handle m_Handle;

        public:
            IBO(std::vector<uint>&, BufferUsage);

            void Bind();

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
                void Bind();

            };

        public:
            std::vector<Element> m_Elements;

        public:
            Mesh() = default;
            Mesh(std::string&);

        };

    private:
        std::vector<std::string> m_SupportedExtensions;

    public:
        Graphics(Window&);
        Graphics() = default;

        void UpdateSurface(Window::Extent);
        void Draw(uint, uint, DrawMode);
        void Clear(glm::vec3);

    };

    template<class T, usz ResourcePoolSize>
    class ResourceLoader {

        private:
            std::string m_ResourceDirectory;
            std::array<T, ResourcePoolSize> m_Resources;
            usz m_ResourcesLast{0};
            std::unordered_map<std::string, usz> m_Map;

        public:
            ResourceLoader(std::string resource_directory) : m_ResourceDirectory(resource_directory) {}

            T& Get(std::string& path) {

                auto empl = m_Map.try_emplace(path, ResourcePoolSize);
                auto it = empl.first;
                auto added = empl.second;

                if(added) {

                    std::string rdir_path = fmt::format("{}/{}", m_ResourceDirectory, path);
                    Debug(fmt::format("Loading resource at `{}`", rdir_path));

                    m_Resources[m_ResourcesLast] = std::move(T(rdir_path));
                    it->second = m_ResourcesLast++;

                }

                return m_Resources[it->second];

            }

    };

    using ImageLoader = ResourceLoader<Graphics::Image, 1024>;
    using ShaderUnitLoader = ResourceLoader<Graphics::Shader::Unit, 1024>;
    using MeshLoader = ResourceLoader<Graphics::Mesh, 1024>;
    using MaterialLoader = ResourceLoader<Graphics::Material, 1024>;

    class Map {

    public:
        std::vector<Graphics::Mesh*> m_Geometry;
        std::vector<glm::vec3> m_PointLightPositions;
        std::vector<glm::vec3> m_PointLightColors;

    public:
        Map(std::string&, ImageLoader&, ShaderUnitLoader&, MeshLoader&, MaterialLoader&);

        void Draw(Graphics&, Graphics::Shader&);

    };

}
