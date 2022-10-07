#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <iostream>
#include <cmath>
#include <array>

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"


namespace airlib_wrapper {
    class __controller_base {
        typedef float _F;
        typedef bool _B;
        typedef std::array<_F, 3> _St;
        typedef msr::airlib::MultirotorRpcLibClient _Cli;
        typedef msr::airlib::Vector3r _Vec;
        typedef msr::airlib::DrivetrainType _Dri;
        typedef msr::airlib::YawMode _Yaw;
        
    private:
        _Cli __cli_as;
        _St __s_;
        _B __r_;
        _B __c_;
        
    public:
        __controller_base(const char *__ip = "");
        ~__controller_base();
        
        void confirm_connection();
        void wait(_F __dur, _B __h = true);
        void hover(_B __s = true);
        void reset();
        void cancel();
        void set_speed(_F __s_h, _F __s_v, _F __s_y);
        void normalize_speed(_F &__x, _F &__y, _F &__z, _F &__r);
        
        void arm();
        void disarm();
        void takeoff(_F __time = 10.0f, _B __sync = true);
        void land(_F __time = 10.0f, _B __sync = true);
        void home(_F __time = 60.0f, _B __sync = true);
        void rotate(_F __yaw, _F __dur, _B __sync = true);
        void rotate_to(_F __deg, _F __time = 10.0f, _B __sync = true);
        void move(_Vec &&__v, _F __dur, int __dri = 1, _F __yaw = 0.0f, _B __sync = true);
        void move_to(_Vec &&__v, _F __vel = 0.0f, _B __sync = true);
        void move_path(std::vector<_Vec> &__v, _F __vel = 0.0f, _F __time = 60.0f, _B __sync = true);
        
        _B is_running();
        _B is_connected();
    };
}

#endif /* __CONTROLLER_BASE_H__ */
