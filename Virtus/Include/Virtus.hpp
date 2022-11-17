#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
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

    void Fatal(std::string);
    void Error(std::string);
    void Info(std::string);

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

        private:
            uint m_UnderlyingHandle;

        public:
            GLHandle() {};
            GLHandle(std::nullptr_t) {};
            GLHandle(uint handle) : m_UnderlyingHandle(handle) {}

            operator uint() { return m_UnderlyingHandle; }

            explicit operator bool() { return m_UnderlyingHandle != 0; }
            bool operator!=(std::nullptr_t) { return m_UnderlyingHandle != 0; }

        };

        class Shader;
        class Shader {

        public:
            class Unit {

            public:
                enum class Stage {

                    Vertex,
                    Fragment

                };

            private:
                Stage m_Stage;
                uint m_Handle;

            public:
                Unit(Stage, std::string&);
                ~Unit();

                friend class Shader;
    
            };

        private:
            uint m_Handle;
            std::unordered_map<std::string, uint> m_Uniforms;

        public:
            Shader(std::vector<Graphics::Shader::Unit>&);
            ~Shader();

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
                ~Member() = default;

            };

        public:
            std::vector<Member> m_Layout;

        public:
            BufferLayout(std::vector<Member>& layout) : m_Layout(layout) {};
            ~BufferLayout() = default;

            void Push(Member);

        };

        class VAO;

        class VBO {

        private:
            uint m_Handle;

        private:
            VBO(void* data, uint size, Graphics::BufferUsage usage);

        public:
            ~VBO();

            void Bind();

            friend class VAO;

        };

        class VAO {

        private:
            uint m_Handle;
            BufferLayout m_Layout;

            std::vector<VBO> m_VBOs;

            bool m_LayoutApplied = false;

        private:
            void ApplyLayout();

        public:
            VAO(BufferLayout);
            ~VAO();

            void Bind();

            VBO& CreateVBO(void*, uint, BufferUsage);

        };

        class IBO {

        private:
            uint m_Handle;

        public:
            IBO(std::vector<uint>&, BufferUsage);
            ~IBO();

            void Bind();

        };

    private:
        std::vector<std::string> m_SupportedExtensions;

    public:
        Graphics(Window&);
        Graphics() = default;

        void Draw(uint, uint, DrawMode);

    };

    class Context {

    public:
        std::shared_ptr<Graphics> m_Graphics;
        std::shared_ptr<Window> m_Window;

    public:
        Context(std::shared_ptr<Window>, std::shared_ptr<Graphics>);
        ~Context() = default;

    };

}
