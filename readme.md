# Smartphone camera remote shutter trigger button
**ESP32_phone_camera_remote_shutter**

The only thing it does is described in the header. Although there are a view tweakable parameters in settings.h.
Tested with Open Camera on Android 8.0.
It possible works with IPhone either.

### How does it work?
1. If you haven't done yet, install [Open Camera](https://play.google.com/store/apps/details?id=net.sourceforge.opencamera) App on your Android device.
2. On your smartphone, turn Bluetooth on.
3. Switch on the ESP32 gadget.
4. Wait for it and confirm the connection.
5. It enumerates as HID keyboard device.
6. If you push the button on your self made remote control, it sends a keystroke to your phone, hence the shutter triggers.

### Wiring:
Due to its simplicity, there is no wiring diagram or schematic. Just have a look at [this picture](https://github.com/michaelruck/ESP32_phone_camera_remote_shutter/blob/master/images/BLE_Shutter_button_v1.0.jpg).

### Hardware:
- ESP32 mini, or any other ESP32.
- Battery compartment 4.5V (3xAAA).
- Tactile switch button.
- Pull up resistor 10k to 50k will work.
- Double sided adhesive tape.

### Dependencies:
- Arduino IDE
- Library

### Ressources:
- Open Camera help, section "Remote control": http://opencamera.org.uk/help.html
- USB HID Usage Tables, Page 53: https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
- Bluetooth HID Profile, Page 20: https://www.silabs.com/documents/login/application-notes/AN993.pdf
- Bluetooth.org Specifications: https://www.bluetooth.org/en-us/specification/adopted-specifications

Author: Michael Ruck, 9. June 2019

License: GNU Lesser General Public License version 3 (https://opensource.org/licenses/LGPL-3.0)
