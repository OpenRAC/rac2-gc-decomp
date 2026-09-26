/* include/core/critical.h */
#ifndef CORE_CRITICAL_H
#define CORE_CRITICAL_H

#include <stdint.h>

uint32_t core_critical_enter(uint32_t intc_id);
void     core_critical_exit(uint32_t prev_state);

#endif
