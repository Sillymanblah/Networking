
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../network/socket.hpp"

int main()
{
	std::cout << "Hello, World!\n";
	try { network::socket server; }
	catch ( const std::runtime_error& error ) { std::cerr << error.what(); }
	std::cout << "Goodbye, World!\n";
	return 0;
}