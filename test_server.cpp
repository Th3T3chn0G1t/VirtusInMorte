#include "test_interface.h"

class Server {

private:
    class Session : public std::enable_shared_from_this<Session> {

    private:
        static const std::size_t DataMax = 1024;

    private:
        tcp::socket m_Socket;
        std::array<char, DataMax> m_Data;

    public:
        Session(tcp::socket socket) : m_Socket(std::move(socket)) {}

        void Start() {

            DoRead();

        }

        void DoRead() {

            auto self(shared_from_this());
            auto read = [this, self](asio::error_code error, std::size_t length) {

                if(!error) {

                    DoWrite(length);

                }
                else {

                    std::cerr << error.message() << std::endl;

                }

            };

            m_Socket.async_read_some(asio::buffer(m_Data, DataMax), read);

        }

        void DoWrite(const std::size_t length) {

            auto self(shared_from_this());
            auto write = [this, self](asio::error_code error, std::size_t length) {

                if(!error) {

                    DoRead();

                }
                else {

                    std::cerr << error.message() << std::endl;

                }

            };

            asio::async_write(m_Socket, asio::buffer(m_Data, length), write);

        }

    };

private:
    tcp::acceptor m_Acceptor;
    tcp::socket m_Socket;

public:
    Server(asio::io_context& io_context, short port) : m_Acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_Socket(io_context) {

        Accept();

    }

private:
    void Accept() {

        auto acceptor = [this](asio::error_code error) {

            if (!error) {

                std::make_shared<Session>(std::move(m_Socket))->Start();
                Accept();

            }
            else {

                std::cerr << error.message() << std::endl;

            }

        };

        m_Acceptor.async_accept(m_Socket, acceptor);

    }

};

int main() {

    asio::io_context io_context;

    Server(io_context, 9999);

    io_context.run();

}
