#include "AirSimControl.h"


AirSimController::AirSimController(_Str __in, int __port)
: _Rpc::server(__in, __port) {
    __a_ = std::thread(&AirSimController::__background_function, this);
    __a_.detach();
    
    __f_.INSERT("SPEED",        &AirSimController::setSpeed   );
    __f_.INSERT("KEYBOARD",     &AirSimController::keyboard   );
    __f_.INSERT("ARM",          &AirSimController::arm        );
    __f_.INSERT("DISARM",       &AirSimController::disarm     );
    __f_.INSERT("TAKEOFF",      &AirSimController::takeoff    );
    __f_.INSERT("LAND",         &AirSimController::land       );
    __f_.INSERT("HOME",         &AirSimController::home       );
    __f_.INSERT("MOVE",         &AirSimController::move       );
    __f_.INSERT("ROTATE",       &AirSimController::rotate     );
    
    if (__in == "stdin") {
        __i_ = _In::STDIN;
        set_signal_handler();
    }
    else {
        __i_ = _In::RPC;
        std::cout.rdbuf(__o_.rdbuf());
        __srv_->bind("str_cmd", _Rpc::_Fp(BIND(&AirSimController::__message_callback)));
        __srv_->async_run();
    }
}

AirSimController::~AirSimController() {
    KeyboardInput::getInstance()->stopCallback();
    if (__i_ == _In::RPC) __srv_->stop();
}


void AirSimController::arm(_Arg &__str) {
    airlib_controller::arm();
}

void AirSimController::disarm(_Arg &__str) {
    airlib_controller::disarm();
}

void AirSimController::setSpeed(_Arg &__str) {
    switch (__str.size()) {
        case 3:
            switch (__str[1][0]) {
                case 'H': set_speed(to_float(__str[2]), -1, -1);
                case 'V': set_speed(-1, to_float(__str[2]), -1);
                case 'R': set_speed(-1, -1, to_float(__str[2]));
                default: std::cout << "Invalid arguments" << "\n" << "\n";
            }
        case 4: set_speed(to_float(__str[1]), to_float(__str[2]), to_float(__str[3])); break;
        default: std::cout << "Invalid arguments" << "\n" << "\n";
    }
}


void AirSimController::keyboard(_Arg &__str) {
    std::cout << "Keyboard mode entered" << "\n";
    std::array<float, 4> __d;
    auto __k = KeyboardInput::getInstance();
    
    while (true) {
        __d.fill(0.0);
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_W)) __d[0] =  0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_A)) __d[1] = -0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_S)) __d[0] = -0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_D)) __d[1] =  0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_Z)) __d[2] = -0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_X)) __d[2] =  0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_Q)) __d[3] = -0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_E)) __d[3] =  0.5f;
        if (__k->isKeyPressed(KEY_CODE::KEY_CODE_F)) break;
        airlib_controller::move({__d[0], __d[1], __d[2]}, 0.1, 1, __d[3], 0);
        airlib_controller::wait(0.1, false);
    }
    airlib_controller::hover();
}

void AirSimController::takeoff(_Arg &__str) {
    switch (__str.size()) {
        case 1: airlib_controller::takeoff(); break;
        case 2: airlib_controller::takeoff(to_float(__str[1])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::land(_Arg &__str) {
    switch (__str.size()) {
        case 1: airlib_controller::land(); break;
        case 2: airlib_controller::land(to_float(__str[1])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::home(_Arg &__str) {
    switch (__str.size()) {
        case 1: airlib_controller::home(); break;
        case 2: airlib_controller::home(to_float(__str[1])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::rotate(_Arg &__str) {
    switch (__str.size()) {
        case 3: if (to_upper(__str[1]) == "TO") rotateTo(__str);
                else airlib_controller::rotate(to_float(__str[1]), to_float(__str[2])); break;
        case 4: if (to_upper(__str[1]) == "TO") rotateTo(__str); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::rotateTo(_Arg &__str) {
    switch (__str.size()) {
        case 3: airlib_controller::rotate_to(to_float(__str[2])); break;
        case 4: airlib_controller::rotate_to(to_float(__str[2]), to_float(__str[3])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::move(_Arg &__str) {
    switch (__str.size()) {
        case 3:
        case 4: if (to_upper(__str[1]) == "PATH") movePath(__str); break;
        case 5: if (to_upper(__str[1]) == "PATH") movePath(__str);
                else if (to_upper(__str[1]) == "TO") moveTo(__str);
                else airlib_controller::move(_Vec(to_float(__str[1]),
                                                  to_float(__str[2]),
                                                  to_float(__str[3])),
                                                  to_float(__str[4])); break;
        case 6: if (to_upper(__str[1]) == "TO") moveTo(__str);
                else airlib_controller::move(_Vec(to_float(__str[1]),
                                                  to_float(__str[2]),
                                                  to_float(__str[3])),
                                                  to_float(__str[4]),
                                                  atoi(__str[5].c_str())); break;
        case 7: airlib_controller::move(_Vec(to_float(__str[1]),
                                             to_float(__str[2]),
                                             to_float(__str[3])),
                                             to_float(__str[4]),
                                             atoi(__str[5].c_str()),
                                             atoi(__str[6].c_str())); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::moveTo(_Arg &__str) {
    switch (__str.size()) {
        case 5: airlib_controller::move_to(_Vec(to_float(__str[2]),
                                                to_float(__str[3]),
                                                to_float(__str[4]))); break;
        case 6: airlib_controller::move_to(_Vec(to_float(__str[2]),
                                                to_float(__str[3]),
                                                to_float(__str[4])),
                                                to_float(__str[5])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void AirSimController::movePath(_Arg &__str) {
    _Str __f_p(std::filesystem::current_path().string());
    __f_p.append("/").append(__str[2]);
    std::vector<_Vec> __p;
    _Str __buf, __buf_n;
    _Vec __v;
    std::ifstream __f(__f_p);
    
    if (!__f) {
        std::cout << "Failed to open file, " << __str[2] << "\n"
            << "Path : " << __f_p << "\n\n";
        return;
    }
    std::cout << "Reading file...";
    
    while (std::getline(__f, __buf)) {
        std::istringstream __ss(__buf);
        for (int i = 0; i < 3; i++) {
            std::getline(__ss, __buf_n, ' ');
            if (__buf_n.empty()) {
                std::cout << "Invalid argument found" << "\n";
                return;
            }
            __v[i] = std::atof(__buf_n.c_str());
        }
        __p.push_back(__v);
    }
    std::cout << "Reading complete" << "\n";
    __f.close();
    
    switch (__str.size()) {
        case 3: airlib_controller::move_path(__p); break;
        case 4: airlib_controller::move_path(__p, to_float(__str[3])); break;
        case 5: airlib_controller::move_path(__p, to_float(__str[3]), to_float(__str[4])); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}


AirSimController::_Str AirSimController::action(const _Str &__str) {
    if (is_running()) {
        std::cout << "Drone is still working on privious task" << "\n";
    }
    if (__str.empty()) return __str;
    std::istringstream __ss(__str);
    _Str __buf;
    std::vector<_Str> __r;
    
    while (std::getline(__ss, __buf, ' ')) {
        __r.push_back(__buf);
    }
    __r[0] = to_upper(__r[0]);
    
    auto __i = __f_.find(__r[0]);
    if (__i != __f_.end()) __i->second(__r);
    else {
        if (__r[0] == "EXIT") {
            if (__i_ == _In::RPC) {
                std::cout << "Shutting down server\n";
            }
            return __r[0];
        }
        std::cout << "Invalid command : " << __str << "\n" << "\n";
    }
    return __r[0];
}

void AirSimController::run() {
    _Str __op;
    while (__op != "EXIT") {
        while (!is_connected()) {
            thread_sleep(1.0f);
        }
        switch (__i_) {
            case STDIN:
                std::cout << "COMMAND > ";
                std::getline(std::cin, __op);
                __op = action(__op);
                break;
            case RPC:
                __l_[0].second.wait();
                __op = action(__l_[0].second.get());
                __l_[1].first.set_value(__o_.str());
                reset_promise(0);
                __o_.str(_Str());
                break;
            default: break;
        }
    }
}


void AirSimController::__background_function() {
    auto __kb = KeyboardInput::getInstance()->runCallback();
    while (__kb->isRunning()) {
//        if (__kb->isKeyPressed(KEY_CODE_C)) {
//            if (__is_running()) __cancel_();
//        }
        if (__kb->isKeyPressed(KEY_CODE::KEY_CODE_R) &&
            __kb->isKeyPressed(KEY_CODE::KEY_CODE_T) &&
            is_connected()) {
            airlib_controller::reset();
            __kb->reset();
        }
        thread_sleep(10);
    }
}

_Rpc::_Str AirSimController::__message_callback(_Str &__msg) {
    __l_[0].first.set_value(__msg);
    __l_[1].second.wait();
    _Str __ret = __l_[1].second.get();
    reset_promise(1);
    return __ret;
}
