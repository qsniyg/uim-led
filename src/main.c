#include "uim.h"
#include "led.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

int main(int argc, char** argv)
{
  char* current;
  regex_t regex;
  int is_print = 0;

  if (argc < 2)
    {
      printf("usage: %s im_name_regex|print\n", argv[0]);
      return 1;
    }

  if (!strcmp(argv[1], "print"))
    is_print = 1;
  else if (regcomp(&regex, argv[1], 0))
    {
      printf("invalid regex: %s\n", argv[1]);
      return 2;
    }

  uimled_uim_init();
  uimled_uim_proplist();

  while ((current = uimled_uim_getcurrent_lock()))
    {
      if (is_print)
        {
          puts(current);
          goto endloop;
        }

      if (!regexec(&regex, current, 0, NULL, 0))
          uimled_led_set(UIMLED_LED_SCROLL_LOCK);
      else
          uimled_led_set(0);

    endloop:
      uimled_uim_getcurrent_unlock();
    }

  return 0;
}
