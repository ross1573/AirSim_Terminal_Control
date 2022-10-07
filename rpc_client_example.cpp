#include <iostream>
#include <thread>
#include <chrono>
#include "rpc/client.h"


int main() {
    rpc::client cli("127.0.0.1", 8080);
    std::cout << "Waiting for server";
    while (cli.get_connection_state() != rpc::client::connection_state::connected) {
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::duration<float>(0.5f));
    }
    std::cout << "\nConnected to Server!" << std::endl;
    
    std::string buf;
    while (buf != "exit_cli") {
        std::cout << "COMMAND > ";
        std::getline(std::cin, buf);
        std::cout << cli.call("str_cmd", buf).as<std::string>();
    }
}
