#pragma once

#include <cglm/cglm.h>

typedef struct Vwdview Vwdview;

#include "../../camcon2/include/camcon2.h"
#include "../../wlezwrap/include/mview.h"
#include "../../wlezwrap/include/wlezwrap.h"

struct Vwdview {
	Wlezwrap wew;
	WlezwrapMview wewmv;
	bool quit;
	bool resize;
	void *data;
	void (*event)(Vwdview* vv, uint8_t type, WlezwrapEvent *event);
	Camcon2 camcon;
	uint32_t window_size[2];
};

void vwdview_build_camera(Vwdview *vv, mat4 view);
void vwdview_s2w(Vwdview *vv, vec2 s, vec2 w);
void vwdview_init(Vwdview* vv);
void vwdview_deinit(Vwdview* vv);
bool vwdview_flush_events(Vwdview* vv);
