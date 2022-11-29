#include "test_interface.h"

int main() {

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("localhost", "9999");
    tcp::socket socket(io_context);
    asio::error_code error;
    asio::connect(socket, endpoints);

    std::string cmd;
    std::array<char, 1024> buffer;
    while(true) {

        cmd.clear();
        buffer.fill('\0');

        std::cin >> cmd;

        send_packet(socket, cmd);

        socket.read_some(asio::buffer(buffer), error);
        error_if(error);

        fmt::print("> {}\n", buffer.data());

        if(cmd == "exit") {

            fmt::print("Exiting...\n");
            return 0;

        }

    }

}
