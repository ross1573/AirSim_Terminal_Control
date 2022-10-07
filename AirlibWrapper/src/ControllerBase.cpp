#include "ControllerBase.h"


airlib_wrapper::__controller_base::__controller_base(const char* ip)
: __cli_as(ip) {
    __s_ = { 1.0f, 1.0f, 30.0f };
    __r_ = false;
    __c_ = false;
    confirm_connection();
}

airlib_wrapper::__controller_base::~__controller_base() {
    cancel();
    __cli_as.enableApiControl(false);
}


void airlib_wrapper::__controller_base::confirm_connection() {
    __cli_as.confirmConnection();
    __cli_as.enableApiControl(true);
    __c_ = true;
}

void airlib_wrapper::__controller_base::wait(float __dur, bool __h) {
    __cli_as.waitOnLastTask(nullptr, __dur);
    if (__h) hover();
    __r_ = false;
}

void airlib_wrapper::__controller_base::hover(_B __s) {
    __r_ = true;
    __cli_as.hoverAsync()->waitOnLastTask();
    if (__s) __cli_as.moveByVelocityAsync(0,0,0, std::numeric_limits<float>::max());
    __r_ = false;
}

void airlib_wrapper::__controller_base::reset() {
    __cli_as.reset();
    __c_ = false;
    __r_ = false;
    confirm_connection();
}

void airlib_wrapper::__controller_base::cancel() {
    __cli_as.cancelLastTask();
    hover(false);
    __r_ = false;
}

void airlib_wrapper::__controller_base::set_speed(float __sh, float __sv, float __sy) {
    if (__sh >= 0.0f) __s_[0] = __sh;
    if (__sv >= 0.0f) __s_[1] = __sv;
    if (__sy >= 0.0f) __s_[2] = __sy;
}

void airlib_wrapper::__controller_base::normalize_speed(float &__x, float &__y, float &__z, float &__r) {
    float __l = std::sqrtf(std::powf(__x, 2.0) + std::powf(__y, 2.0f));
    if (__l != 0 && __l != 1) {
        __x /= __l;
        __y /= __l;
    }
    __x *= __s_[0];
    __y *= __s_[0];
    if      (__z > 0.0f) __z = -__s_[1];
    else if (__z < 0.0f) __z =  __s_[1];
    if      (__r > 0.0f) __r =  __s_[2];
    else if (__r < 0.0f) __r = -__s_[2];
}


void airlib_wrapper::__controller_base::arm() {
#ifndef DISABLE_LOG
    std::cout << "Arm executed\n\n";
#endif
    __cli_as.armDisarm(true);
}

void airlib_wrapper::__controller_base::disarm() {
#ifndef DISABLE_LOG
    std::cout << "Disarm executed\n\n";
#endif
    __cli_as.armDisarm(false);
}

void airlib_wrapper::__controller_base::takeoff(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Takeoff executed" << "\n"
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_as.takeoffAsync(__time);
    if (__sync) wait(__time);
}

void airlib_wrapper::__controller_base::land(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Land executed" << "\n"
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_as.landAsync(__time);
    if (__sync) wait(__time);
}

void airlib_wrapper::__controller_base::home(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Home executed" << "\n"
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_as.goHomeAsync(__time);
    if (__sync) wait(__time);
}

void airlib_wrapper::__controller_base::rotate(float __yaw, float __dur, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Rotate executed" << "\n"
        << "Yaw : " << __yaw << "\n"
        << "Duration : " << __dur << "\n\n";
#endif
    __r_ = true;
    __cli_as.rotateByYawRateAsync(__yaw, __dur);
    if (__sync) wait(__dur);
}

void airlib_wrapper::__controller_base::rotate_to(float __deg, float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Rotate executed" << "\n"
        << "Degree : " << __deg << "\n"
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_as.rotateToYawAsync(__deg, __time);
    if (__sync) wait(__time);
}

void airlib_wrapper::__controller_base::move(_Vec &&__v, float __dur, int __dri, float __yaw, bool __sync) {
    normalize_speed(__v[0], __v[1], __v[2], __yaw);
#ifndef DISABLE_LOG
    if (std::pow(__v[0], 2) + std::pow(__v[1], 2) + std::pow(__v[2], 2) + std::pow(__yaw, 2) > 0.01f) {
        std::cout << "Move executed" << "\n"
            << "Velocity : " << __v[0] << ", " << __v[1] << ", " << __v[2] << ", " << __yaw << "\n"
            << "Duration : " << __dur << "\n"
            << "DriveTrain : " << (__dri ? "ForwoardOnly" : "MaxDegreeOfFreedom") << "\n"
            << "Async : " << std::boolalpha << __sync << "\n\n";
    }
#endif
    __r_ = true;
//    __cli_as.moveByRollPitchYawZAsync(__v[0], __v[1], __v[2], -1.0f, __dur);
//    __cli_as.moveByRollPitchYawrateZAsync(__v[0], __v[1], __v[2], -1.0f, __dur);
//    __cli_as.moveByVelocityAsync(__v[0], __v[1], __v[2], __dur, (_Dri)__dri, _Yaw(true, __yaw));
    __cli_as.moveByVelocityBodyFrameAsync(__v[0], __v[1], __v[2], __dur, (_Dri)__dri, _Yaw(true, __yaw));
    if (__sync) wait(__dur);
}

void airlib_wrapper::__controller_base::move_to(_Vec &&__v, float __vel, bool __sync) {
    if (__v[2] < 0.0f) {
#ifndef DISABLE_LOG
        std::cout << "Invalid arguments\n\n";
#endif
        return;
    }
    auto __p = __cli_as.getMultirotorState().getPosition();
    if (!__vel) __vel = __s_[0];
    
#ifndef DISABLE_LOG
    std::cout << "MoveTo executed" << "\n"
        << "X : " <<  __p.x() << " > " << __v[0] << "\n"
        << "Y : " <<  __p.y() << " > " << __v[1] << "\n"
        << "Z : " << -__p.z() << " > " << __v[2] << "\n"
        << "Velocity : " << __vel << "\n" << "\n";
#endif
    
    __r_ = true;
    __cli_as.moveToPositionAsync(__v[0], __v[1], -__v[2], __vel);
    if (__sync) wait(std::numeric_limits<float>::max());
}

void airlib_wrapper::__controller_base::move_path(std::vector<_Vec> &__v, float __vel, float __time, bool __sync) {
    if (!__vel) __vel = __s_[0];
    for (auto &ele : __v) ele[2] *= -1;
    __r_ = true;
    __cli_as.moveOnPathAsync(__v, __vel, __time, (_Dri)1, _Yaw(false, 0));
    
#ifndef DISABLE_LOG
    std::cout << "MovePath executed" << "\n" << "Path : \t";
    for (auto v : __v) {
        std::cout << (v.x() < 0 ? "" : " ") << v.x() << ",\t"
            << (v.y() < 0 ? "" : " ") << v.y() << ",\t"
            << -v.z() << "\n\t\t";
    }
    std::cout << "\n";
#endif
    if (__sync) wait(__time);
}


airlib_wrapper::__controller_base::_B airlib_wrapper::__controller_base::is_running() {
    return __r_;
}

airlib_wrapper::__controller_base::_B airlib_wrapper::__controller_base::is_connected() {
    return __c_;
}
