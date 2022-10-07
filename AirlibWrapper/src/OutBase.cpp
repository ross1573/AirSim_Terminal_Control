#include "OutBase.h"


airlib_wrapper::__sensor_base::__sensor_base(const char *__ip)
: __cli_as(__ip) {
    confirm_connection();
    __cli_as.enableApiControl(true);
}

airlib_wrapper::__sensor_base::~__sensor_base() {
    __cli_as.enableApiControl(false);
}


void airlib_wrapper::__sensor_base::confirm_connection() {
    __cli_as.confirmConnection();
}


airlib_wrapper::__sensor_base::_Li airlib_wrapper::__sensor_base::lidar() {
    return __cli_as.getLidarData();
}

airlib_wrapper::__sensor_base::_Imu airlib_wrapper::__sensor_base::imu() {
    return __cli_as.getImuData();
}

airlib_wrapper::__sensor_base::_Bar airlib_wrapper::__sensor_base::barometer() {
    return __cli_as.getBarometerData();
}

airlib_wrapper::__sensor_base::_Mag airlib_wrapper::__sensor_base::magnetometer() {
    return __cli_as.getMagnetometerData();
}

airlib_wrapper::__sensor_base::_Gps airlib_wrapper::__sensor_base::gps() {
    return __cli_as.getGpsData();
}

airlib_wrapper::__sensor_base::_Dis airlib_wrapper::__sensor_base::distance() {
    return __cli_as.getDistanceSensorData();
}
