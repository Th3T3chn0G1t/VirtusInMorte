#include "test_interface.h"

int main() {

    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 9999));

    asio::error_code error;

    while(true) {

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::array<char, 1024> buffer;
        while(true) {
            buffer.fill('\0');

            socket.read_some(asio::buffer(buffer), error);
            error_if(error);

            std::cout << buffer.data() << std::endl;

            std::string ret;
            if(std::string("ping") == buffer.data()) ret = "pong";
            else if(std::string("exit") == buffer.data()) {

                socket.shutdown(asio::socket_base::shutdown_both, error);
                error_if(error);
                socket.close(error);
                error_if(error);

                break;

            }
            else ret = "unknown command";

            send_packet(socket, ret);

        }

    }

}
