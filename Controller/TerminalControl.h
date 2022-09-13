#ifndef __TERMINAL_CONTROL_H__
#define __TERMINAL_CONTROL_H__

#include <array>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <thread>
#include <chrono>
#include <filesystem>

#include "../KeyboardInput/KeyboardInput.h"
#include "ControllerBase.h"


class TerminalController : private __controller_base {
    typedef std::vector<std::string> _Arg;
    typedef std::function<void(_Arg&)> _Fp;
    typedef std::map<std::string, _Fp> _Fm;
    typedef msr::airlib::Vector3r _Vec;
    
private:
    std::thread __a_;
    _Fm __f_;
    
public:
    TerminalController();
    ~TerminalController();
    
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
    
    std::string action(std::string &__str);
    void wait();
    void run();
    
private:
    void __aot_function();
    std::string __to_upper(const std::string &__str);
};

#endif /* __TERMINAL_CONTROL_H__ */
