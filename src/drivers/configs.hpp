#pragma once
#ifndef NETWORK_CONFIGS_HPP
#define NETWORK_CONFIGS_HPP

#include "../network/socket"

constexpr network::AF address_family = network::AF::INET;
constexpr network::PF protocol_family = network::PF::INET;
constexpr network::PROTOCOL ip_protocol = network::PROTOCOL::TCP;
const network::socket_address server_address( address_family, 192, 168, 1, 150, 10000 );

#endif // NETWORK_CONFIGS_HPP