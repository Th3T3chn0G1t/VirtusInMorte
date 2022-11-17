#include <Virtus.hpp>

namespace Virtus {

    void Fatal(std::string message) {

        fmt::print(fmt::fg(fmt::color::purple), "Fatal: {}\n", message);
        std::abort();

    }

    void Error(std::string message) {

        fmt::print(fmt::fg(fmt::color::yellow), "Error: {}\n", message);

    }

    void Info(std::string message) {

        fmt::print(fmt::fg(fmt::color::green), "Info: {}\n", message);

    }

}

