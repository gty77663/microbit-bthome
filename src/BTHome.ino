/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by pcbreflux
   Modified to work with NimBLE
   Modified for V2 by Chreece
   Modified by countrysideboy: Code cleanups, Chop Data, Encryption
   BLE advertisement format from https://bthome.io/

*/

#include <stdint.h>
#include "Arduino.h"
#include "BTHome.h"

#define DEVICE_NAME "MicroBit-bthome"
#define TEXT_FIELD  "TEST STRING"

#define DEBOUNCE_TIME 200
#define SEND_DELAY 5000

BTHome bthome;

volatile uint8_t buttonAState = 0;
volatile uint8_t buttonBState = 0;

void setup() {
  Serial.begin(115200);
  bthome.begin(DEVICE_NAME, false, "", false);

  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);
  //attachInterrupt(PIN_BUTTON_A, buttonADebounce, CHANGE);
  //attachInterrupt(PIN_BUTTON_B, buttonBDebounce, CHANGE);
}

void loop() {
  static u_long local_timer = millis();
  float temp = read_temp();

  if (millis() - local_timer > SEND_DELAY) {
    bthome.resetMeasurement();
    bthome.addMeasurement(ID_TEMPERATURE_PRECISE, temp); 
    bthome.addMeasurement_state(STATE_POWER_ON, STATE_ON);
    bthome.addMeasurement_state(EVENT_BUTTON, !digitalRead(PIN_BUTTON_A)); 
    
    String msg = TEXT_FIELD;
    bthome.addMeasurement(ID_TEXT, (uint8_t *)msg.c_str(), msg.length());

    bthome.sendPacket();
    bthome.stop();

    local_timer = millis();
  }
}

float read_temp() {
  NRF_TEMP->TASKS_START = 1;
  while (!NRF_TEMP->EVENTS_DATARDY);
  NRF_TEMP->EVENTS_DATARDY = 0;
  int temp = NRF_TEMP->TEMP;
  NRF_TEMP->TASKS_STOP = 1;

  return (float)(temp) / 4;
}

void buttonADebounce() {
  buttonAState = !digitalRead(PIN_BUTTON_A);
}

void buttonBDebounce() {
  buttonBState = !digitalRead(PIN_BUTTON_B);
}


//Object ids by order
#if 0
#define ID_PACKET				0x00
#define ID_BATTERY				0x01
#define ID_TEMPERATURE_PRECISE 	0x02
#define ID_HUMIDITY_PRECISE 	0x03
#define ID_PRESSURE 			0x04
#define ID_ILLUMINANCE			0x05
#define ID_MASS					0x06
#define ID_MASSLB				0x07
#define ID_DEWPOINT				0x08
#define ID_COUNT				0x09
#define ID_ENERGY				0x0A
#define ID_POWER				0x0B
#define ID_VOLTAGE				0x0C
#define ID_PM25					0x0D
#define ID_PM10					0x0E
#define STATE_GENERIC_BOOLEAN	0x0F
#define STATE_POWER_ON			0x10
#define STATE_OPENING			0x11
#define ID_CO2					0x12
#define ID_TVOC					0x13
#define ID_MOISTURE_PRECISE		0x14
#define STATE_BATTERY_LOW		0x15
#define STATE_BATTERY_CHARHING	0x16
#define STATE_CO				0x17
#define STATE_COLD				0x18
#define STATE_CONNECTIVITY		0x19
#define STATE_DOOR				0x1A
#define STATE_GARAGE_DOOR		0x1B
#define STATE_GAS_DETECTED		0x1C
#define STATE_HEAT				0x1D
#define STATE_LIGHT				0x1E
#define STATE_LOCK				0x1F
#define STATE_MOISTURE			0x20
#define STATE_MOTION			0x21
#define STATE_MOVING			0x22
#define STATE_OCCUPANCY			0x23
#define STATE_PLUG				0x24
#define STATE_PRESENCE			0x25
#define STATE_PROBLEM			0x26
#define STATE_RUNNING			0x27
#define STATE_SAFETY			0x28
#define STATE_SMOKE				0x29
#define STATE_SOUND				0x2A
#define STATE_TAMPER			0x2B
#define STATE_VIBRATION			0x2C
#define STATE_WINDOW			0x2D
#define ID_HUMIDITY				0x2E
#define ID_MOISTURE				0x2F
#define EVENT_BUTTON			0x3A
#define EVENT_DIMMER			0x3C
#define ID_COUNT2				0x3D
#define ID_COUNT4				0x3E
#define ID_ROTATION				0x3F
#define ID_DISTANCE				0x40
#define ID_DISTANCEM			0x41
#define ID_DURATION				0x42
#define ID_CURRENT				0x43
#define ID_SPD					0x44
#define ID_TEMPERATURE			0x45
#define ID_UV					0x46
#define ID_VOLUME1				0x47
#define ID_VOLUME2				0x48
#define ID_VOLUMEFR				0x49
#define ID_VOLTAGE1				0x4A
#define ID_GAS					0x4B
#define ID_GAS4					0x4C
#define ID_ENERGY4				0x4D
#define ID_VOLUME				0x4E
#define ID_WATER				0x4F
#endif
