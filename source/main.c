#include "ps4.h"

int _main(struct thread *td) {
  UNUSED(td);

  initKernel();
  initLibc();

  printf_notification("Hello World");

  return 0;
}
