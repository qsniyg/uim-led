#include "led.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

/* thanks to https://logixoul.wordpress.com/2008/08/14/how-to-set-leds-in-x-even-on-ubuntu/ */
void uimled_led_set(int leds)
{
  Display *dpy = XOpenDisplay(0);

  XKeyboardControl values;
  values.led_mode = leds & UIMLED_LED_SCROLL_LOCK ? LedModeOn : LedModeOff;
  values.led = 3;

  XChangeKeyboardControl(dpy, KBLedMode, &values);

  XFlush(dpy);
  XCloseDisplay(dpy);
}
