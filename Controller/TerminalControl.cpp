#define INSERT(x, y) insert(_Fm::value_type(x, std::bind(y, this, std::placeholders::_1)))
#include "TerminalControl.h"


TerminalController::TerminalController() {
    KeyboardInput::getInstance()->runCallback();
    __confirm_connection();
    __a_ = std::thread(&TerminalController::__aot_function, this);
    __a_.detach();
    
    __f_.INSERT("SPEED",        &TerminalController::setSpeed   );
    __f_.INSERT("KEYBOARD",     &TerminalController::keyboard   );
    __f_.INSERT("ARM",          &TerminalController::arm        );
    __f_.INSERT("DISARM",       &TerminalController::disarm     );
    __f_.INSERT("TAKEOFF",      &TerminalController::takeoff    );
    __f_.INSERT("LAND",         &TerminalController::land       );
    __f_.INSERT("HOME",         &TerminalController::home       );
    __f_.INSERT("MOVE",         &TerminalController::move       );
    __f_.INSERT("ROTATE",       &TerminalController::rotate     );
}

TerminalController::~TerminalController() {
    KeyboardInput::getInstance()->stopCallback();
}


void TerminalController::arm(_Arg &__str) {
    __arm_();
}

void TerminalController::disarm(_Arg &__str) {
    __disarm_();
}

void TerminalController::setSpeed(_Arg &__str) {
    switch (__str.size()) {
        case 3:
            switch (__str[1][0]) {
                case 'H': __set_speed(atof(__str[2].c_str()), -1, -1);
                case 'V': __set_speed(-1, atof(__str[2].c_str()), -1);
                case 'R': __set_speed(-1, -1, atof(__str[2].c_str()));
                default: std::cerr << "Invalid arguments" << std::endl << std::endl;
            }
        case 4: __set_speed(atof(__str[1].c_str()), atof(__str[2].c_str()), atof(__str[3].c_str())); break;
        default: std::cerr << "Invalid arguments" << std::endl << std::endl;
    }
}


void TerminalController::keyboard(_Arg &__str) {
    std::cout << "Keyboard mode entered" << std::endl;
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
        __move_({__d[0], __d[1], __d[2]}, 0.1, 1, __d[3], 0);
        __wait_(0.1, false);
    }
    __hover_();
}

void TerminalController::takeoff(_Arg &__str) {
    switch (__str.size()) {
        case 1: __takeoff_(); break;
        case 2: __takeoff_(atof(__str[1].c_str())); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}

void TerminalController::land(_Arg &__str) {
    switch (__str.size()) {
        case 1: __land_(); break;
        case 2: __land_(atof(__str[1].c_str())); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}

void TerminalController::home(_Arg &__str) {
    switch (__str.size()) {
        case 1: __home_(); break;
        case 2: __home_(atof(__str[1].c_str())); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}

void TerminalController::rotate(_Arg &__str) {
    switch (__str.size()) {
        case 3: if (__to_upper(__str[1]) == "TO") rotateTo(__str);
                else __rotate_(atof(__str[1].c_str()), atof(__str[2].c_str())); break;
        case 4: if (__to_upper(__str[1]) == "TO") rotateTo(__str); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}

void TerminalController::rotateTo(_Arg &__str) {
    switch (__str.size()) {
        case 3: __rotate_to(atof(__str[2].c_str())); break;
        case 4: __rotate_to(atof(__str[2].c_str()), atof(__str[3].c_str())); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}

void TerminalController::move(_Arg &__str) {
    switch (__str.size()) {
        case 3:
        case 4: if (__to_upper(__str[1]) == "PATH") movePath(__str); break;
        case 5: if (__to_upper(__str[1]) == "PATH") movePath(__str);
                else if (__to_upper(__str[1]) == "TO") moveTo(__str);
                else __move_(_Vec(atof(__str[1].c_str()),
                             atof(__str[2].c_str()),
                             atof(__str[3].c_str())),
                             atof(__str[4].c_str())); break;
        case 6: if (__to_upper(__str[1]) == "TO") moveTo(__str);
                else __move_(_Vec(atof(__str[1].c_str()),
                             atof(__str[2].c_str()),
                             atof(__str[3].c_str())),
                             atof(__str[4].c_str()),
                             atoi(__str[5].c_str())); break;
        case 7: __move_(_Vec(atof(__str[1].c_str()),
                        atof(__str[2].c_str()),
                        atof(__str[3].c_str())),
                        atof(__str[4].c_str()),
                        atoi(__str[5].c_str()),
                        atoi(__str[6].c_str())); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void TerminalController::moveTo(_Arg &__str) {
    switch (__str.size()) {
        case 5: __move_to(_Vec(atof(__str[2].c_str()),
                          atof(__str[3].c_str()),
                          atof(__str[4].c_str()))); break;
        case 6: __move_to(_Vec(atof(__str[2].c_str()),
                          atof(__str[3].c_str()),
                          atof(__str[4].c_str())),
                          atof(__str[5].c_str())); break;
        default: std::cout << "Invalid arguments\n\n";
    }
}

void TerminalController::movePath(_Arg &__str) {
    std::string __f_p(std::filesystem::current_path().string());
    __f_p.append("/").append(__str[2]);
    std::vector<_Vec> __p;
    std::string __buf, __buf_n;
    _Vec __v;
    std::ifstream __f(__f_p);
    
    if (!__f) {
        std::cerr << "Failed to open file, " << __str[2] << "\n"
        << "Path : " << __f_p << "\n\n";
        return;
    }
    std::cout << "Reading file...";
    
    while (std::getline(__f, __buf)) {
        std::istringstream __ss(__buf);
        for (int i = 0; i < 3; i++) {
            std::getline(__ss, __buf_n, ' ');
            if (__buf_n.empty()) {
                std::cerr << "Invalid argument found" << std::endl;
                return;
            }
            __v[i] = std::atof(__buf_n.c_str());
        }
        __p.push_back(__v);
    }
    std::cout << "Reading complete" << std::endl;
    __f.close();
    
    switch (__str.size()) {
        case 3: __move_path(__p); break;
        case 4: __move_path(__p, atof(__str[3].c_str())); break;
        case 5: __move_path(__p, atof(__str[3].c_str()), atof(__str[4].c_str())); break;
        default: std::cerr << "Invalid arguments\n\n";
    }
}


std::string TerminalController::action(std::string &__str) {
    if (__is_running()) {
        std::cout << "Drone is still working on privious task" << std::endl;
    }
    if (__str.empty()) return __str;
    std::istringstream __ss(__str);
    std::string __buf;
    std::vector<std::string> __r;
    
    while (std::getline(__ss, __buf, ' ')) {
        __r.push_back(__buf);
    }
    __r[0] = __to_upper(__r[0]);
    
    auto __i = __f_.find(__r[0]);
    if (__i != __f_.end()) __i->second(__r);
    else {
        if (__r[0] == "EXIT") return __r[0];
        std::cerr << "Invalid command : " << __str << std::endl << std::endl;
    }
    return __r[0];
}

void TerminalController::run() {
    std::string __op;
    while (__op != "EXIT") {
        while (!__is_connected()) {
            std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
        }
        std::cout << "COMMAND > ";
        std::getline(std::cin, __op);
        __op = action(__op);
    }
}


void TerminalController::__aot_function() {
    auto __kb = KeyboardInput::getInstance();
    while (__kb->isRunning()) {
//        if (__kb->isKeyPressed(KEY_CODE_C)) {
//            if (__is_running()) __cancel_();
//        }
        if (__kb->isKeyPressed(KEY_CODE::KEY_CODE_R) &&
            __kb->isKeyPressed(KEY_CODE::KEY_CODE_T) &&
            __is_connected()) {
            __reset_();
            __kb->reset();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

std::string TerminalController::__to_upper(const std::string &__str) {
    std::string __up = __str;
    std::transform(__up.begin(), __up.end(), __up.begin(),
                   [](char __c) { return std::toupper(__c); });
    return __up;
}
