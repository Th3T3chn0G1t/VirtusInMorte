#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <functional>
#include <filesystem>
#include <exception>
#include <fstream>
#include <optional>
#include <random>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
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
    using half = ushort; // Symbolic - storage type only
    using uptr = std::uintptr_t;
    using usz = std::size_t;

    [[noreturn]] void Fatal(const std::string&);
    void Error(const std::string&);
    void Info(const std::string&);
    void Debug(const std::string&);

    template<class T>
    static typename std::underlying_type<T>::type EnumToUnderlying(const T value) {

        return static_cast<typename std::underlying_type<T>::type>(value);

    }

    template<typename T>
    class Span {

    public:
        T* m_Data;
        usz m_Size;

    public:
        Span(T* start, T* end) : m_Data(start), m_Size(end - start) {}
        Span(T* start, usz size) : m_Data(start), m_Size(size) {}

        template<class S>
        Span(S& container) : m_Data(container.data()), m_Size(container.size()) {}

    };

    template<class T, usz ResourcePoolSize>
    class ResourceLoader {

    private:
        std::string m_ResourceDirectory;
        std::array<T, ResourcePoolSize> m_Resources;
        usz m_ResourcesLast{0};
        std::unordered_map<std::string, usz> m_Map;

    public:
        ResourceLoader(const std::string& resource_directory) : m_ResourceDirectory(resource_directory) {}

        template<class B>
        T& Get(const std::string& path, B& bundle) {

            auto emplaced = m_Map.try_emplace(path, ResourcePoolSize);
            auto it = emplaced.first;
            auto added = emplaced.second;

            if(added) {

                std::string rdir_path {fmt::format("{}/{}", m_ResourceDirectory, path)};
                Debug(fmt::format("Loading resource at `{}`", rdir_path));

                m_Resources[m_ResourcesLast] = std::move(T(rdir_path, bundle));
                it->second = m_ResourcesLast++;

            }

            return m_Resources[it->second];

        }

    };

    class Item {

    public:
        enum class Usage {

            Consumable,
            Reusable,
            Equipable

        };

        enum class Slot {

            None,
            Weapon,
            Armor

        };

        enum class Rarity {

            Common,
            Uncommon,
            Rare

        };

    public:
        Usage m_Usage;
        Slot m_Slot;
        Rarity m_Rarity;
        std::string m_Name;
        std::string m_Description;

    public:
        Item(Usage usage, Slot slot, Rarity rarity, const std::string& name, const std::string& description) : m_Usage(usage), m_Slot(slot), m_Rarity(rarity), m_Name(name), m_Description(description) {}

        virtual void OnUse() {}

        virtual void OnEquip() {}
        virtual void OnDisarm() {}

        virtual void OnAdd() {}
        virtual void OnRemove() {}

    };

    namespace Protocol {

        enum class Operation : ushort {

            Null = 0,
            Echo = 65,
            Broadcast = 66

        };

        class Packet {

        public:
            static const usz PacketDataLength = 1024 - sizeof(Operation);

        private:
            union Data {

                std::array<uchar, PacketDataLength> u_EchoMessage;
                std::array<uchar, PacketDataLength> u_Pad;

            };

        public:
            Operation m_Operation;
            Data m_Data;

        };

    }

}
