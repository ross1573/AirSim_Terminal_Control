#include "../include/Util.h"


_Rpc::server::server(_Str __ip, int __port) {
    if (__ip == "stdin") return;
    if (__ip == "localhost") __ip = "127.0.0.1";
    try {
        __srv_ = _Rpc::_Srv(new rpc::server(__ip, __port));
    }
    catch (std::exception __e) {
        std::cout << "Failed to create server, "
            << "Please check ip address\n";
        exit(EXIT_FAILURE);
    }
    
    reset_promise(0);
    reset_promise(1);
}

void _Rpc::server::reset_promise(int __i) {
    __l_[__i].first = _Rpc::_Prm();
    __l_[__i].second = __l_[__i].first.get_future();
}


_Rpc::client::client(_Str __ip, int __port) {
    if (__ip == "localhost") __ip = "127.0.0.1";
    __cli_ = _Cli(new rpc::client(__ip, __port));
    std::cout << "Waiting for server.";
    while (__cli_->get_connection_state() != rpc::client::connection_state::connected) {
        std::cout << ".";
        thread_sleep(0.5f);
    }
    std::cout << "\nConnected to server" << std::endl;
}


void signal_handler(int sig) {
    std::cout << "\nPress Enter to exit" << std::endl;
    std::istringstream iss("EXIT\n");
    std::cin.rdbuf(iss.rdbuf());
}

void set_signal_handler() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGABRT, signal_handler);
#ifdef __APPLE__ 
    std::signal(SIGSTOP, signal_handler);
    std::signal(SIGQUIT, signal_handler);
    std::signal(SIGKILL, signal_handler);
    std::signal(SIGTSTP, signal_handler);
#elif _WIN32
    std::signal(SIGBREAK, signal_handler);
#endif
}

void thread_sleep(float __s) {
    std::this_thread::sleep_for(std::chrono::duration<float>(__s));
}

void thread_sleep(int __ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(__ms));
}

std::string to_upper(const std::string &__str) {
    std::string __up = __str;
    std::transform(__up.begin(), __up.end(), __up.begin(),
                   [](char __c) { return std::toupper(__c); });
    return __up;
}

float to_float(const std::string &__str) {
    return static_cast<float>(atof(__str.c_str()));
}
