#include "ControllerBase.h"


__controller_base::__controller_base() {
    __s_ = { 1.0f, 1.0f, 30.0f };
    __r_ = false;
    __c_ = false;
}

__controller_base::~__controller_base() {
    __cancel_();
    __cli_.enableApiControl(false);
}


void __controller_base::__confirm_connection() {
    __cli_.confirmConnection();
    __cli_.enableApiControl(true);
    __c_ = true;
}

void __controller_base::__wait_(float __dur, bool __h) {
    __cli_.waitOnLastTask(nullptr, __dur);
    if (__h) __hover_();
    __r_ = false;
}

void __controller_base::__hover_(_B __s) {
    __r_ = true;
    __cli_.hoverAsync()->waitOnLastTask();
    if (__s) __cli_.moveByVelocityAsync(0,0,0, std::numeric_limits<float>::max());
    __r_ = false;
}

void __controller_base::__reset_() {
    __cli_.reset();
    __c_ = false;
    __r_ = false;
    __confirm_connection();
}

void __controller_base::__cancel_() {
    __cli_.cancelLastTask();
    __hover_(false);
    __r_ = false;
}

void __controller_base::__set_speed(float __sh, float __sv, float __sy) {
    if (__sh >= 0.0f) __s_[0] = __sh;
    if (__sv >= 0.0f) __s_[1] = __sv;
    if (__sy >= 0.0f) __s_[2] = __sy;
}

void __controller_base::__normalize_speed(float &__x, float &__y, float &__z, float &__r) {
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


void __controller_base::__arm_() {
#ifndef DISABLE_LOG
    std::cout << "Arm executed\n\n";
#endif
    __cli_.armDisarm(true);
}

void __controller_base::__disarm_() {
#ifndef DISABLE_LOG
    std::cout << "Disarm executed\n\n";
#endif
    __cli_.armDisarm(false);
}

void __controller_base::__takeoff_(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Takeoff executed" << std::endl
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_.takeoffAsync(__time);
    if (__sync) __wait_(__time);
}

void __controller_base::__land_(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Land executed" << std::endl
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_.landAsync(__time);
    if (__sync) __wait_(__time);
}

void __controller_base::__home_(float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Home executed" << std::endl
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_.goHomeAsync(__time);
    if (__sync) __wait_(__time);
}

void __controller_base::__stop_(float __dur) {
    __hover_();
    std::this_thread::sleep_for(std::chrono::duration<float>(__dur));
}

void __controller_base::__rotate_(float __yaw, float __dur, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Rotate executed" << std::endl
        << "Yaw : " << __yaw << std::endl
        << "Duration : " << __dur << "\n\n";
#endif
    __r_ = true;
    __cli_.rotateByYawRateAsync(__yaw, __dur);
    if (__sync) __wait_(__dur);
}

void __controller_base::__rotate_to(float __deg, float __time, bool __sync) {
#ifndef DISABLE_LOG
    std::cout << "Rotate executed" << std::endl
        << "Degree : " << __deg << std::endl
        << "Timeout : " << __time << "\n\n";
#endif
    __r_ = true;
    __cli_.rotateToYawAsync(__deg, __time);
    if (__sync) __wait_(__time);
}

void __controller_base::__move_(_Vec &&__v, float __dur, int __dri, float __yaw, bool __sync) {
    __normalize_speed(__v[0], __v[1], __v[2], __yaw);
#ifndef DISABLE_LOG
    if (std::pow(__v[0], 2) + std::pow(__v[1], 2) + std::pow(__v[2], 2) + std::pow(__yaw, 2) > 0.01f) {
        std::cout << "Move executed" << std::endl
            << "Velocity : " << __v[0] << ", " << __v[1] << ", " << __v[2] << ", " << __yaw << std::endl
            << "Duration : " << __dur << std::endl
            << "DriveTrain : " << (__dri ? "ForwoardOnly" : "MaxDegreeOfFreedom") << std::endl
            << "Async : " << std::boolalpha << __sync << "\n\n";
    }
#endif
    __r_ = true;
//    __cli_.moveByRollPitchYawZAsync(__v[0], __v[1], __v[2], -1.0f, __dur);
//    __cli_.moveByRollPitchYawrateZAsync(__v[0], __v[1], __v[2], -1.0f, __dur);
//    __cli_.moveByVelocityAsync(__v[0], __v[1], __v[2], __dur, (_Dri)__dri, _Yaw(true, __yaw));
    __cli_.moveByVelocityBodyFrameAsync(__v[0], __v[1], __v[2], __dur, (_Dri)__dri, _Yaw(true, __yaw));
    if (__sync) __wait_(__dur);
}

void __controller_base::__move_to(_Vec &&__v, float __vel, bool __sync) {
    if (__v[2] < 0.0f) {
#ifndef DISABLE_LOG
        std::cout << "Invalid arguments\n\n";
#endif
        return;
    }
    auto __p = __cli_.getMultirotorState().getPosition();
    if (!__vel) __vel = __s_[0];
    
#ifndef DISABLE_LOG
    std::cout << "MoveTo executed" << std::endl
        << "X : " <<  __p.x() << " > " << __v[0] << std::endl
        << "Y : " <<  __p.y() << " > " << __v[1] << std::endl
        << "Z : " << -__p.z() << " > " << __v[2] << std::endl
        << "Velocity : " << __vel << std::endl << std::endl;
#endif
    
    __r_ = true;
    __cli_.moveToPositionAsync(__v[0], __v[1], -__v[2], __vel);
    if (__sync) __wait_(std::numeric_limits<float>::max());
}

void __controller_base::__move_path(std::vector<_Vec> &__v, float __vel, float __time, bool __sync) {
    if (!__vel) __vel = __s_[0];
    for (auto &ele : __v) ele[2] *= -1;
    __r_ = true;
    __cli_.moveOnPathAsync(__v, __vel, __time, (_Dri)1, _Yaw(false, 0));
    
#ifndef DISABLE_LOG
    std::cout << "MovePath executed" << std::endl << "Path : \t";
    for (auto v : __v) {
        std::cout << (v.x() < 0 ? "" : " ") << v.x() << ",\t"
            << (v.y() < 0 ? "" : " ") << v.y() << ",\t"
            << -v.z() << "\n\t\t";
    }
    std::cout << std::endl;
#endif
    if (__sync) __wait_(__time);
}


bool __controller_base::__is_running() {
    return __r_;
}

bool __controller_base::__is_connected() {
    return __c_;
}

__controller_base::_Li __controller_base::__data_lidar() {
    auto __d = __cli_.getLidarData();
    
    return __d;
}

__controller_base::_Imu __controller_base::__data_imu() {
    auto __d = __cli_.getImuData();
    
    return __d;
}

__controller_base::_Bar __controller_base::__data_barometer() {
    auto __d = __cli_.getBarometerData();
    
    return __d;
}

__controller_base::_Mag __controller_base::__data_magnet() {
    auto __d = __cli_.getMagnetometerData();
    
    return __d;
}

__controller_base::_Gps __controller_base::__data_gps() {
    auto __d = __cli_.getGpsData();
    
    return __d;
}

__controller_base::_Dis __controller_base::__data_distance() {
    auto __d = __cli_.getDistanceSensorData();
    
    return __d;
}
