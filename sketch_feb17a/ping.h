#ifndef __PING_SENSOR_H__
#define __PING_SENSOR_H__

namespace ping_sensor {
    struct ping_sensor {
      int trig_pin;
      int echo_pin;
    };
    typedef struct ping_sensor ping_sensor;

    ping_sensor init(int trig, int echo);
    void set_trig_pin(ping_sensor ping, int trig);
    void set_echo_pin(ping_sensor ping, int echo);
    long measure_distance(ping_sensor ping);
}

#endif