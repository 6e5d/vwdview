#include <cglm/cglm.h>

#include "../../camcon2/build/camcon2.h"
#include "../../wlezwrap/build/wlezwrap.h"
#include "../../vwdlayer/build/vwdlayer.h"

// input state
// 0 null
// 1 mclick: pan
// 2 shift>mclick: rotate
// 3 ctrl>mclick: zoom
// 4 lclick: draw
// 5 shift>lclick: size
typedef struct {
	void (*cb_submit)(void*);
	void (*cb_undo)(void*, bool);
	void (*cb_key)(void*, uint8_t, bool);
	void *data;

	Wlezwrap() wew;
	uint32_t input_state;
	float pps[3];
	bool skip; // first event after press
	Vwdlayer(Ifdraw) ifdraw;
	// only 1 undo/redo op allowed in 1 frame
	// undo=1, redo=-1
	bool quit;
	bool resize;
	void *brush;
	Camcon2() camcon;
	uint32_t window_size[2];
	int32_t offset[2]; // of current focus, for c2l mapping
} Vwdview();

void vwdview(build_camera)(Vwdview() *vv, mat4 view);
void vwdview(s2w)(Vwdview() *vv, vec2 s, vec2 w);
void vwdview(init)(Vwdview()* vv);
void vwdview(deinit)(Vwdview()* vv);
bool vwdview(flush_events)(Vwdview()* vv);

void vwdview(event)(void* data, uint8_t type, Wlezwrap(Event) *e);
