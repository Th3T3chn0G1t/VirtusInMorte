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

#include <fmt/format.h>
#include <fmt/color.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <yaml-cpp/yaml.h>
#include <asio.hpp>
using asio::ip::tcp;

namespace Common {

    using uchar = unsigned char;
    using ushort = unsigned char;
    using uint = unsigned int;
    using half = __fp16;
    using uptr = std::uintptr_t;
    using usz = std::size_t;

    void Fatal(std::string);
    void Error(std::string);
    void Info(std::string);
    void Debug(std::string);

    template<typename E>
    constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {

        return static_cast<std::underlying_type_t<E>>(e);

    }

    template<typename E>
    requires std::is_enum<E>::value
    constexpr inline bool bit_enum = false;

    template<typename E>
    requires bit_enum<E>
    constexpr E operator&(E left, E right) noexcept {

        return static_cast<E>(to_underlying(left) & to_underlying(right));

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E operator|(E left, E right) noexcept {

        return static_cast<E>(to_underlying(left) | to_underlying(right));

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E operator^(E left, E right) noexcept {

        return static_cast<E>(to_underlying(left) ^ to_underlying(right));

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E operator~(E value) noexcept {

        return static_cast<E>(~to_underlying(value));

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E& operator&=(E& left, E right) noexcept {

        return left = left & right;

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E& operator|=(E& left, E right) noexcept {

        return left = left | right;

    }

    template<typename E>
    requires bit_enum<E>
    constexpr E& operator^=(E& left, E right) noexcept {

        return left = left ^ right;

    }

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

                std::string rdir_path {fmt::format("{}/{}", m_ResourceDirectory, path)};
                Debug(fmt::format("Loading resource at `{}`", rdir_path));

                m_Resources[m_ResourcesLast] = std::move(T(rdir_path));
                it->second = m_ResourcesLast++;

            }

            return m_Resources[it->second];

        }

    };

    namespace Game {

        class Item {

        };

    }

}
