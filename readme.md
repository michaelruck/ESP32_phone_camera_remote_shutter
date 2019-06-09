# Smartphone camera remote shutter trigger button
**ESP32_phone_camera_remote_shutter**

The only thing it does is described in the header. Although there are a view tweakable parameters in settings.h.
Tested with Open Camera on Android 8.0.
It possible works with Iphone either.

Ressources:
- Bluetooth HID Profile, Page 20: https://www.silabs.com/documents/login/application-notes/AN993.pdf
- USB HID Usage Tables, Page 53: https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
- Bluetooth.org Specifications https://www.bluetooth.org/en-us/specification/adopted-specifications

Hardware:
- ESP32 mini, or any other ESP32.
- Battery compartment 4.5V (3xAAA).
- Tactile switch button.
- Pull up resistor 10k to 50k will work.
- Double sided adhesive tape.

Author: Michael Ruck, 9. Juny 2019

License: GNU Lesser General Public License version 3 (https://opensource.org/licenses/LGPL-3.0)
