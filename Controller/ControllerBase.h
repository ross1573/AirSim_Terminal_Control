#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>
#include <thread>
#include <chrono>

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"

using namespace msr;


class __controller_base {
    typedef airlib::MultirotorRpcLibClient _Cli;
    typedef std::array<float, 3> _St;
    typedef airlib::Vector3r _Vf;
    
private:
    _Cli __c_;
    _St __s_;
    bool __r_;
    
protected:
    __controller_base();
    ~__controller_base();
    
    void __confirm_connection();
    void __wait_(bool __h = true);
    void __hover_();
    void __reset_();
    void __cancel_();
    void __set_speed(float __s_h, float __s_v, float __s_y);
    void __normalize_speed(float &__x, float &__y, float &__z, float &__r);
    
    void __arm_();
    void __disarm_();
    void __takeoff_(float __time = 10.0f, bool __sync = true);
    void __land_(float __time = 10.0f, bool __sync = true);
    void __home_(float __time = 60.0f, bool __sync = true);
    void __stop_(float __dur);
    void __rotate_(float __yaw, float __dur, bool __sync = true);
    void __rotate_to(float __yaw, float __time = 10.0f, bool __sync = true);
    void __move_(_Vf &&__v, float __dur, int __dri = 1, float __yaw = 0.0f, bool __sync = true);
    void __move_to(_Vf &&__v, float __vel = 0.0f, bool __sync = true);
    void __move_path(std::vector<_Vf> &__v, float __vel = 0.0f, float __time = 60.0f, bool __sync = true);
    
    inline bool __is_running();
    inline airlib::LidarData __data_lidar();
    inline airlib::ImuBase::Output __data_imu();
    inline airlib::BarometerBase::Output __data_barometer();
    inline airlib::MagnetometerBase::Output __data_magnet();
    inline airlib::GpsBase::Output __data_gps();
    inline airlib::DistanceSensorData __data_distance();
};

#endif /* __CONTROLLER_BASE_H__ */
