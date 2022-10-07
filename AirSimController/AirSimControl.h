#ifndef __AIRSIM_CONTROL_H__
#define __AIRSIM_CONTROL_H__

#define MSGPACK_DISABLE_LEGACY_NIL
#define BIND(x) std::bind(x, this, std::placeholders::_1)
#define INSERT(x, y) insert(_Fm::value_type(x, BIND(y)))

#include <array>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <future>
#include <functional>
#include <filesystem>

#include "KeyboardInput/KeyboardInput.h"
#include "AirlibWrapper.h"


class AirSimController : private airlib_controller, _Rpc::server {
    typedef std::string                     _Str;
    typedef std::thread                     _Thd;
    typedef msr::airlib::Vector3r           _Vec;
    typedef std::vector<_Str>               _Arg;
    typedef std::function<void(_Arg&)>      _Fp;
    typedef std::map<std::string, _Fp>      _Fm;
    
private:
    _Thd __a_;
    _Fm __f_;
    _In __i_;
    
public:
    AirSimController(_Str __in, int __port);
    ~AirSimController();
    
    void arm(_Arg &__str);
    void disarm(_Arg &__str);
    void setSpeed(_Arg &__str);
    void keyboard(_Arg &__str);
    void takeoff(_Arg &__str);
    void land(_Arg &__str);
    void home(_Arg &__str);
    void rotate(_Arg &__str);
    void rotateTo(_Arg &__str);
    void move(_Arg &__str);
    void moveTo(_Arg &__str);
    void movePath(_Arg &__str);
    
    _Str action(const _Str &__str);
    void reset();
    void run();
    
private:
    void __background_function();
    _Str __message_callback(_Str &__msg);
};

#endif /* __AIRSIM_CONTROL_H__ */
