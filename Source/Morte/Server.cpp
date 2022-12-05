namespace Morte {

    class Server {

    private:
        static Protocol::Packet OnUnhandledPacket(const Protocol::Packet& packet) {

            Error(fmt::format("Unhandled packet for operation {}", EnumToUnderlying(packet.m_Operation)));

            return Protocol::Packet{Protocol::Operation::Null, {}};

        }

    private:
        using OperationHandlers = std::unordered_map<Protocol::Operation, std::function<Protocol::Packet(const Protocol::Packet&)>>;

        class Session : public std::enable_shared_from_this<Session> {

        private:
            usz m_ID;
            tcp::socket m_Socket;
            Protocol::Packet m_Data;
            std::reference_wrapper<Server> m_Server;

        public:
            explicit Session(tcp::socket socket, Server& server, const usz id) : m_Socket(std::move(socket)), m_Data(), m_Server(server), m_ID(id) {}

            ~Session() {

                for(auto it = m_Server.get().m_Sessions.begin(); it != m_Server.get().m_Sessions.end(); it++) {

                    if(it->get().m_ID == m_ID) {

                        m_Server.get().m_Sessions.erase(it);
                        return;

                    }

                }

            }

            void Start() {

                Await();

            }

            void Await() {

                auto self(shared_from_this());
                auto read = [this, self](asio::error_code error, std::size_t length) {

                    if(!error) {

                        auto it = m_Server.get().m_Handlers.find(m_Data.m_Operation);
                        auto handler = it != m_Server.get().m_Handlers.end() ? it->second : OnUnhandledPacket;
                        Send(handler(m_Data));
                        Await();

                    }
                    else {

                        Error(error.message());

                    }

                };

                std::memset(&m_Data, 0, sizeof(Protocol::Packet));
                m_Socket.async_read_some(asio::buffer(&m_Data, sizeof(Protocol::Packet)), read);

            }

            // TODO: SendUntil - minimize wastage by only sending until a stop byte
            void Send(const Protocol::Packet& packet) {

                auto self(shared_from_this());
                auto write = [this, self](asio::error_code error, std::size_t length) {

                    if(error) {

                        Error(error.message());

                    }

                };

                asio::async_write(m_Socket, asio::buffer(&packet, sizeof(Protocol::Packet)), write);

            }

        };

    private:
        tcp::acceptor m_Acceptor;
        tcp::socket m_Socket;
        OperationHandlers m_Handlers;
        std::vector<std::reference_wrapper<Session>> m_Sessions;
        usz m_CurrentID{0};

    public:
        Server(asio::io_context& io_context, short port) : m_Acceptor(io_context, tcp::endpoint(tcp::v4(), port)), m_Socket(io_context) {

            Accept();

            m_Handlers[Protocol::Operation::Null] = [&](const Protocol::Packet& packet)-> Protocol::Packet {

                return {Protocol::Operation::Null, {}};

            };

            m_Handlers[Protocol::Operation::Echo] = [&](const Protocol::Packet& packet)-> Protocol::Packet {

                return {Protocol::Operation::Echo, packet.m_Data};

            };

            m_Handlers[Protocol::Operation::Broadcast] = [&](const Protocol::Packet& packet)-> Protocol::Packet {

                Broadcast(packet);
                return {Protocol::Operation::Null, {}};

            };

        }

        void Broadcast(const Protocol::Packet& packet) {

            for(auto& session : m_Sessions) {

                session.get().Send(packet);

            }

        }

    private:
        void Accept() {

            auto acceptor = [this](asio::error_code error) {

                if (!error) {

                    auto session = std::make_shared<Session>(std::move(m_Socket), *this, m_CurrentID++);
                    m_Sessions.push_back(*session);
                    session->Start();
                    Accept();

                }
                else {

                    std::cerr << error.message() << std::endl;

                }

            };

            m_Acceptor.async_accept(m_Socket, acceptor);

        }

        friend class Session;

    };

}

int main() {

    asio::io_context io_context;

    Morte::Server server(io_context, 9999);

    io_context.run();

    return 0;

}
