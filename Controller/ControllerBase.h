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


class __controller_base {
    typedef float _F;
    typedef bool _B;
    typedef std::array<_F, 3> _St;
    typedef msr::airlib::MultirotorRpcLibClient _Cli;
    typedef msr::airlib::Vector3r _Vec;
    typedef msr::airlib::LidarData _Li;
    typedef msr::airlib::ImuBase::Output _Imu;
    typedef msr::airlib::BarometerBase::Output _Bar;
    typedef msr::airlib::MagnetometerBase::Output _Mag;
    typedef msr::airlib::GpsBase::Output _Gps;
    typedef msr::airlib::DistanceSensorData _Dis;
    typedef msr::airlib::DrivetrainType _Dri;
    typedef msr::airlib::YawMode _Yaw;
    
private:
    _Cli __cli_;
    _St __s_;
    _B __r_;
    _B __c_;
    
protected:
    __controller_base();
    ~__controller_base();
    
    void __confirm_connection();
    void __wait_(_F __dur, _B __h = true);
    void __hover_(_B __s = true);
    void __reset_();
    void __cancel_();
    void __set_speed(_F __s_h, _F __s_v, _F __s_y);
    void __normalize_speed(_F &__x, _F &__y, _F &__z, _F &__r);
    
    void __arm_();
    void __disarm_();
    void __takeoff_(_F __time = 10.0f, _B __sync = true);
    void __land_(_F __time = 10.0f, _B __sync = true);
    void __home_(_F __time = 60.0f, _B __sync = true);
    void __stop_(_F __dur);
    void __rotate_(_F __yaw, _F __dur, _B __sync = true);
    void __rotate_to(_F __deg, _F __time = 10.0f, _B __sync = true);
    void __move_(_Vec &&__v, _F __dur, int __dri = 1, _F __yaw = 0.0f, _B __sync = true);
    void __move_to(_Vec &&__v, _F __vel = 0.0f, _B __sync = true);
    void __move_path(std::vector<_Vec> &__v, _F __vel = 0.0f, _F __time = 60.0f, _B __sync = true);
    
    _B __is_running();
    _B __is_connected();
    _Li __data_lidar();
    _Imu __data_imu();
    _Bar __data_barometer();
    _Mag __data_magnet();
    _Gps __data_gps();
    _Dis __data_distance();
};

#endif /* __CONTROLLER_BASE_H__ */
