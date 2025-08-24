#pragma once
#ifndef SHARED_HPP
#define SHARED_HPP

#include "../network/socket"

constexpr network::AF address_family = network::AF::INET;
constexpr network::PF protocol_family = address_family;
constexpr network::PROTOCOL ip_protocol = network::PROTOCOL::TCP;

#endif // SHARED_HPP