#ifdef __INTELLISENSE
#include <Virtus.hpp>
#endif

namespace Common {

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

    void Debug(std::string message) {

        fmt::print(fmt::fg(fmt::color::light_blue), "Debug: {}\n", message);

    }

}

