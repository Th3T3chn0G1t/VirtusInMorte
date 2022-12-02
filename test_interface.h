#pragma once

#include <asio.hpp>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>
using asio::ip::tcp;

static void error_if(asio::error_code error) {

    if(error) {

        std::cerr << "Asio Error: " << error.message() << std::endl;
        std::abort();

    }

}

static void send_packet(tcp::socket& socket, std::string data) {

    asio::error_code error;
    asio::write(socket, asio::buffer(data), error);
    error_if(error);

}

constexpr std::size_t type_size = 5;
