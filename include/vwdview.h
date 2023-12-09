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
	void (*cb_submit)(void*);
	void (*cb_undo)(void*, bool);
	void (*cb_key)(void*, uint8_t);
	void *data;

	Wlezwrap wew;
	uint32_t input_state;
	float pps[3];
	bool skip; // first event after press
	VwdlayerIfdraw ifdraw;
	// only 1 undo/redo op allowed in 1 frame
	// undo=1, redo=-1
	bool quit;
	bool resize;
	void *brush;
	Camcon2 camcon;
	uint32_t window_size[2];
	int32_t offset[2]; // of current focus, for c2l mapping
};

void vwdview_build_camera(Vwdview *vv, mat4 view);
void vwdview_s2w(Vwdview *vv, vec2 s, vec2 w);
void vwdview_init(Vwdview* vv);
void vwdview_deinit(Vwdview* vv);
bool vwdview_flush_events(Vwdview* vv);

void vwdview_event(void* data, uint8_t type, WlezwrapEvent *e);

#endif
