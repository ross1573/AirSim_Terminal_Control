#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <sstream>
#include <string>
#include <csignal>
#include <stdlib.h>
#include <functional>
#include <memory>
#include <future>
#include <thread>
#include <chrono>
#include <algorithm>

#include "rpc/server.h"
#include "rpc/client.h"


enum _In {
    STDIN,
    RPC
};

struct _Rpc {
    typedef std::string                     _Str;
    typedef std::unique_ptr<rpc::server>    _Srv;
    typedef std::unique_ptr<rpc::client>    _Cli;
    typedef std::function<_Str(_Str&)>      _Fp;
    typedef std::stringstream               _Out;
    typedef std::promise<_Str>              _Prm;
    typedef std::future<_Str>               _Fut;
    typedef std::pair<_Prm, _Fut>           _Syn;
    
    class server {
    protected:
        _Srv __srv_;
        _Out __o_;
        _Syn __l_[2];
        
    protected:
        server(_Str __ip, int __port);
        
        void reset_promise(int __i);
    };
    
    class client {
    protected:
        _Cli __cli_;
        
    protected:
        client(_Str __ip, int __port);
    };
};


void set_signal_handler(std::function<void(int)> __f);
void thread_sleep(float __s);
void thread_sleep(int __ms);
std::string to_upper(const std::string &__str);
float to_float(const std::string &__str);

#endif /* __UTIL_H__ */
