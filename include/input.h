#pragma once

#include <stdint.h>

#include "../../wlezwrap/include/wlezwrap.h"
#include "../../wlezwrap/include/mview.h"

void vwdview_mview_event(WlezwrapMview *wewmv, double x, double y);
void vwdview_event(void* data, uint8_t type, WlezwrapEvent *e);
