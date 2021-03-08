#include <Wire.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <arduino_homekit_server.h>
#include "wifi_config.h"

#define SERVO			12
#define LED				14

// Set the position for calcifer-bot's "finger" (as degrees)
#define PRESS			80		// finger pushing down (goes past 90 to ensure downward pressure)
#define RELEASE			180		// finger sticking up

#define	PRESS_DURATION	3000	// time to hold button, in milliseconds

const char *ssid =  MY_SSID;
const char *password = MY_PASSWORD; 

Servo servo;

int servo_angle = 0;
bool should_enable_led = false;

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t characteristic_on;


void setup() {
	pinMode(LED, OUTPUT);
	
	// connect to the servo and set it to the default position
	servo.attach(SERVO);
	finger_up();
	
	wifi_connect();
	
	calcifer_homekit_setup();
}

void loop() {
	calcifer_homekit_loop();
	led_setter();
	delay(10);
}

void wifi_connect() {
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);
	WiFi.begin(ssid, password);
	
	// block until WiFi is connected
	while(!WiFi.isConnected()){
		delay(100);
	}
}

void characteristic_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	should_enable_led = on;
	characteristic_on.value.bool_value = on;
	press();
}

void calcifer_homekit_setup() {
	characteristic_on.setter = characteristic_on_setter;
	arduino_homekit_setup(&config);
}

void calcifer_homekit_loop() {
	arduino_homekit_loop();
}

void finger_down() {
	servo.write(PRESS);	
}

void finger_up() {
	servo.write(RELEASE);	
}

void press() {
	finger_down();
	delay(PRESS_DURATION);
	finger_up();
}

void led_setter() {
	if (should_enable_led) {
		digitalWrite(LED, HIGH);
	} else {
		digitalWrite(LED, LOW);
	}
}