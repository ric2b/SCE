#include <stdio.h>

extern void cmd_ini (int, char** );
extern void monitor(void);

void monitoring(void)
{
  cmd_ini(0, NULL);
  monitor();

  return 0;
}
