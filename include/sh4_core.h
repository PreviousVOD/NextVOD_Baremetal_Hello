#ifndef SH4_CORE_H
#define SH4_CORE_H

#include <stdint.h>

#define  __trapa(code)  asm volatile("trapa %0" :: "i" (code))

#endif  // SH4_CORE_H
