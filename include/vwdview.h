#ifndef INCLUDEGUARD_VWDVIEW
#define INCLUDEGUARD_VWDVIEW

#include <cglm/cglm.h>

typedef struct Vwdview Vwdview;

#include "../../camcon2/include/camcon2.h"
#include "../../wlezwrap/include/wlezwrap.h"
#include "../../vwdlayer/include/vwdlayer.h"

// input state
// 0 null
// 1 mclick: pan
// 2 shift>mclick: rotate
// 3 ctrl>mclick: zoom
// 4 lclick: draw
// 5 shift>lclick: size
struct Vwdview {
	Wlezwrap wew;
	uint32_t input_state;
	float pps[3];
	VwdlayerIfdraw ifdraw;
	bool quit;
	bool resize;
	void *data;
	Camcon2 camcon;
	uint32_t window_size[2];
	int32_t offset[2]; // of current focus, for c2l mapping
};

void vwdview_build_camera(Vwdview *vv, mat4 view);
void vwdview_s2w(Vwdview *vv, vec2 s, vec2 w);
void vwdview_init(Vwdview* vv);
void vwdview_deinit(Vwdview* vv);
bool vwdview_flush_events(Vwdview* vv);

#endif
