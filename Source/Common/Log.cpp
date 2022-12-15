namespace Common {

    void Fatal(const std::string& message) {

        fmt::print(fmt::fg(fmt::color::purple), "Fatal: {}\n", message);
        std::abort();

    }

    void Error(const std::string& message) {

        fmt::print(fmt::fg(fmt::color::yellow), "Error: {}\n", message);

    }

    void Info(const std::string& message) {

        fmt::print(fmt::fg(fmt::color::green), "Info: {}\n", message);

    }

    void Debug(const std::string &message) {

        fmt::print(fmt::fg(fmt::color::light_blue), "Debug: {}\n", message);

    }

}

