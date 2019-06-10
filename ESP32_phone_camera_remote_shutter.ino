/*
  Smartphone camera remote shutter trigger button

  The main thing it does is described in the header. If you press the button, the shutter is triggered to take an image. 
  With the slide switch you can adapt to the different keycodes Android and IOS use. 
  All necessary parameters are in settings.h.
  Tested with Open Camera on Android 8.0.
  It possible works with Iphone either.

  Ressources:
  Bluetooth HID Profile, Page 20: https://www.silabs.com/documents/login/application-notes/AN993.pdf
  USB HID Usage Tables, Page 53: https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
  Bluetooth.org Specifications https://www.bluetooth.org/en-us/specification/adopted-specifications

  Hardware:
  ESP32 mini, or any other ESP32.
  Battery compartment 4.5V (3xAAA).
  Tactile switch button.
  Pull up resistor 10k to 50k will work.
  Double sided adhesive tape.

  Author: Michael Ruck, 9. Juny 2019
  License: GNU Lesser General Public License version 3 (https://opensource.org/licenses/LGPL-3.0)

*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "./settings.h"

#define LED 2

BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;

bool connected = false;
bool btnFlag = false;

class MyCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      connected = true;
      BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      desc->setNotifications(true);
    }

    void onDisconnect(BLEServer* pServer) {
      connected = false;
      BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      desc->setNotifications(false);
    }
};

/*
   This callback is connect with output report. In keyboard output report report special keys changes, like CAPSLOCK, NUMLOCK
   We can add digital pins with LED to show status
   bit 0 - NUM LOCK
   bit 1 - CAPS LOCK
   bit 2 - SCROLL LOCK
*/
class MyOutputCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* me) {
      uint8_t* value = (uint8_t*)(me->getValue().c_str());
      ESP_LOGI(LOG_TAG, "special keys: %d", *value);
    }
};

void taskServer(void*) {


  BLEDevice::init(__BT_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  hid = new BLEHIDDevice(pServer);
  input = hid->inputReport(1); // <-- input REPORTID from report map
  output = hid->outputReport(1); // <-- output REPORTID from report map

  output->setCallbacks(new MyOutputCallbacks());

  std::string name = __MANUFACTURER;
  hid->manufacturer()->setValue(name);

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x02);

  BLESecurity *pSecurity = new BLESecurity();
  //  pSecurity->setKeySize();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

  const uint8_t report[] = {
    USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
    USAGE(1),           0x06,       // Keyboard
    COLLECTION(1),      0x01,       // Application
    REPORT_ID(1),       0x01,        //   Report ID (1)
    USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
    REPORT_COUNT(1),    0x08,
    HIDINPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
    REPORT_SIZE(1),     0x08,
    HIDINPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    HIDINPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x08,       //   LEDs
    USAGE_MINIMUM(1),   0x01,       //   Num Lock
    USAGE_MAXIMUM(1),   0x05,       //   Kana
    HIDOUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
    REPORT_SIZE(1),     0x03,
    HIDOUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    END_COLLECTION(0)
  };

  hid->reportMap((uint8_t*)report, sizeof(report));
  hid->startServices();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();
  hid->setBatteryLevel(7);

  ESP_LOGD(LOG_TAG, "Advertising started!");
  delay(portMAX_DELAY);

};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE works!");

  // The Button
  pinMode(__BUTTONPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(__BUTTONPIN), pushButton, FALLING);

  // The Switch
  pinMode(__SWITCHPIN, INPUT);

  // LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  xTaskCreate(taskServer, "server", 20000, NULL, 5, NULL);
}

void loop() {

  if (connected & btnFlag) {
    btnFlag = false;
    digitalWrite(LED, HIGH);
    Serial.println("Cheese...");

    uint8_t send_key = digitalRead(__SWITCHPIN) ? __SEND_KEY_ANDROID : __SEND_KEY_IOS;
    
    //Key press
    uint8_t msg[] = {0x0, 0x0, send_key, 0x0, 0x0, 0x0, 0x0, 0x0};
    input->setValue(msg, sizeof(msg));
    input->notify();

    //Key release
    uint8_t msg1[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    input->setValue(msg1, sizeof(msg1));
    input->notify();

    delay(1000);
    digitalWrite(LED, LOW);
    attachInterrupt(digitalPinToInterrupt(__BUTTONPIN), pushButton, FALLING);
  }
  delay(50);
}

IRAM_ATTR void pushButton() {
  detachInterrupt(digitalPinToInterrupt(__BUTTONPIN));
  btnFlag = true;
}
