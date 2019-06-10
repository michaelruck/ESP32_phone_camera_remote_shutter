/*
   Open Camera remote control

  Some remote control buttons and selfie sticks work by transmitting a volume key command, which by default will take a photo, but you can change this from the Settings.

  Open Camera also supports some remote control via a Bluetooth or USB keyboard:

    Function key or numeric keypad "*": toggle popup menu
    "/": toggle exposure menu
    Space or numeric keypad "5": Activate shutter (take photo/video), unless the exposure or popup menu is open, and you are in highlight selection mode (see below).
    Up/down arrows, or numeric keypad "8" and "2": if the exposure or popup menu is open, this enters highlight selection mode:
        For the popup menu, move the highlighted row up or down to select a row, then press Space/"5", then you can change the selected icon in that row with the up/down arrows, then press Space/"5" again to select the button.
        For the exposure menu, move the highlighted row up or down to select a row, then press Space/"5", then you can change the selected value or slider with the up/down arrows.
    "+" or "-": Zoom in or out.

  Source: https://opencamera.sourceforge.io/help.html
  
*/

#define __BUTTONPIN 39
#define __SWITCHPIN 27

#define __MANUFACTURER "Michael Ruck"
#define __BT_NAME "cam-remote-button"

/*
 * If you need another keycode, open hut1_12v2.pdf page 53
 */
#define __SEND_KEY_ANDROID 0x5d //Keypad 5, for Android Open Camera
#define __SEND_KEY_IOS 0x80 //Volume up, for Iphone (not tested)
