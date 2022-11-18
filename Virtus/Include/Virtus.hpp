#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cstdlib>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <fmt/color.h>

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

    };

    class Graphics {

    public:
        class GLHandle {

        public:
            uint m_UnderlyingHandle = 0;

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
            BufferLayout m_Layout;

            std::vector<VBO> m_VBOs;

            bool m_LayoutApplied = false;

        private:
            void ApplyLayout();

        public:
            VAO(BufferLayout);

            void Bind();

            VBO& CreateVBO(void*, uint, BufferUsage);

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

        void Draw(uint, uint, DrawMode);
        void Clear(float[3]);

    };

    class Context {

    public:
        std::shared_ptr<Graphics> m_Graphics;
        std::shared_ptr<Window> m_Window;

    public:
        Context(std::shared_ptr<Window>, std::shared_ptr<Graphics>);

    };

}
