#pragma once

#include <string>
#include <iostream>
#include <vector>
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
#ifdef __INTELLISENSE
                Image(std::string&);
#else
                Image(std::filesystem::path&);
#endif

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
                Unit(Stage, std::string&);

                friend class Shader;
    
            };

        private:
            Handle m_Handle;
            std::unordered_map<std::string, uint> m_Uniforms;

        public:
            Shader(std::vector<Graphics::Shader::Unit>&);

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

        class BufferLayout {

        public:
            class Member {

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
                Member(Type type, uint count, uint divisor) : m_Type(type), m_Count(count), m_Divisor(divisor) {}

            };

        public:
            std::vector<Member> m_Layout;

        public:
            BufferLayout(std::vector<Member>& layout) : m_Layout(layout) {};

            void Push(Member);

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

        public:
            VAO();

            void Bind();

            VBO& CreateVBO(void*, uint, BufferUsage, BufferLayout&);

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

    private:
        std::vector<std::string> m_SupportedExtensions;

    public:
        Graphics(Window&);
        Graphics() = default;

        void UpdateSurface(Window::Extent);
        void Draw(uint, uint, DrawMode);
        void Clear(glm::vec3);

    };

    class Context {

    public:
        std::shared_ptr<Graphics> m_Graphics;
        std::shared_ptr<Window> m_Window;

    public:
        Context(std::shared_ptr<Window>, std::shared_ptr<Graphics>);

    };

}
