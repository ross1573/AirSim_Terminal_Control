#ifndef __OUT_BASE_H__
#define __OUT_BASE_H__

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"


namespace airlib_wrapper {
    class __sensor_base {
        typedef msr::airlib::MultirotorRpcLibClient _Cli;
        typedef msr::airlib::Vector3r _Vec;
        typedef msr::airlib::LidarData _Li;
        typedef msr::airlib::ImuBase::Output _Imu;
        typedef msr::airlib::BarometerBase::Output _Bar;
        typedef msr::airlib::MagnetometerBase::Output _Mag;
        typedef msr::airlib::GpsBase::Output _Gps;
        typedef msr::airlib::DistanceSensorData _Dis;
        
    private:
        _Cli __cli_as;
        
    public:
        __sensor_base(const char *__ip = "");
        ~__sensor_base();
        
        void confirm_connection();
        
        _Li lidar();
        _Imu imu();
        _Bar barometer();
        _Mag magnetometer();
        _Gps gps();
        _Dis distance();
    };
}

#endif /* __OUT_BASE_H__ */
