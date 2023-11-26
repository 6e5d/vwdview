#ifndef INCLUDEGUARD_VWDVIEW_INPUTH
#define INCLUDEGUARD_VWDVIEW_INPUTH

#include <stdint.h>

#include "../../wlezwrap/include/wlezwrap.h"

void vwdview_event(void* data, uint8_t type, WlezwrapEvent *e);

#endif
