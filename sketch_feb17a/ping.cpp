#include "ping_sensor.h"
#include <Arduino.h>


namespace ping_sensor {

  ping_sensor init(int trig, int echo)
  {
    ping_sensor ping;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    ping.echo_pin = echo;
    ping.trig_pin = trig;
    return ping;
  }

  void set_trig_pin(ping_sensor ping, int trig) 
  {
    ping.trig_pin = trig;
  }

  void set_echo_pin(ping_sensor ping, int echo) 
  {
    ping.echo_pin = echo;
  }

  long measure_distance(ping_sensor ping) 
  {
    int trigPin = ping.trig_pin;
    int echoPin = ping.echo_pin;
    long duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
  }
}