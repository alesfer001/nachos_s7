#ifndef __USER_PROCESS__
#define __USER_PROCESS__

#include "userthread.h"

struct procArgs
{
  AddrSpace *space;
  int exit_value;
};

extern int do_ProcessCreate(char* filename, int exit_value);

extern void StartUserProcess(void * schmurtz);

#endif
