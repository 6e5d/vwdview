#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../../wlezwrap/include/wlezwrap.h"
#include "../include/input.h"
#include "../include/vwdview.h"

static float angle_norm(float angle) {
	static const float PI = (float)M_PI;
	while (angle >= PI) {
		angle -= 2.0f * PI;
	}
	while (angle < PI) {
		angle += 2.0f * PI;
	}
	return angle;
}

static void f_key(Vwdview *iv, uint8_t key, bool pressed) {
	if (!pressed) { return; }
	if (key == 'i') {
		iv->camcon.k *= 1.33f;
	} else if (key == 'o') {
		iv->camcon.k /= 1.33f;
	} else {
		return;
	}
}

static void f_resize(Vwdview *vv, uint32_t w, uint32_t h) {
	vv->window_size[0] = w;
	vv->window_size[1] = h;
	vv->resize = true;
}

static void f_quit(Vwdview *vv) {
	vv->quit = true;
}

static void vwdview_mouse_click(Vwdview* vv, uint8_t id, bool updown) {
	bool *keystate = vv->wew.keystate;
	if (updown) {
		if (id == WLEZWRAP_MCLICK) {
			if (keystate[WLEZWRAP_LSHIFT]) {
				vv->input_state = 2;
			} else if (keystate[WLEZWRAP_LCTRL]) {
				vv->input_state = 3;
			} else {
				vv->input_state = 1;
			}
		} else if (id == WLEZWRAP_LCLICK) {
			vv->input_state = 4;
		}
	} else {
		if (id == WLEZWRAP_LCLICK) {
			vv->ifdraw.end(vv->data);
		}
		vv->input_state = 0;
	}
}

static void vwdview_view_pan(Vwdview* vv, WlezwrapEvent *e) {
	vec2 wpos, wpps;
	vwdview_s2w(vv, e->motion, wpos);
	vwdview_s2w(vv, vv->pps, wpps);
	vv->camcon.x += wpos[0] - wpps[0];
	vv->camcon.y += wpos[1] - wpps[1];
}

static void vwdview_view_rotate(Vwdview* vv, WlezwrapEvent *e) {
	float cx = (float)vv->window_size[0] / 2.0f;
	float cy = (float)vv->window_size[1] / 2.0f;
	float t0 = atan2f(vv->pps[1] - cy, vv->pps[0] - cx);
	float t1 = atan2f(e->motion[1] - cy, e->motion[0] - cx);
	t1 = angle_norm(t1 - t0);
	vv->camcon.theta += (float)t1;
}

static void vwdview_view_zoom(Vwdview* vv, WlezwrapEvent *e) {
	vv->camcon.k *=  1.0f - (float)(e->motion[1] - vv->pps[1]) * 0.005f;
}

void vwdview_event(void* data, uint8_t type, WlezwrapEvent *e) {
	Vwdview* vv = data;
	switch(type) {
	case 0:
		f_quit(vv);
		break;
	case 1:
		f_resize(vv, e->resize[0], e->resize[1]);
		break;
	case 2:
		if (vv->input_state == 1) {
			vwdview_view_pan(vv, e);
		} else if (vv->input_state == 2) {
			vwdview_view_rotate(vv, e);
		} else if (vv->input_state == 3) {
			vwdview_view_zoom(vv, e);
		} else if (vv->input_state == 4) {
			float wpos[3], wpps[3];
			memcpy(wpos, &e->motion, 3 * sizeof(float));
			memcpy(wpps, &vv->pps, 3 * sizeof(float));
			vwdview_s2w(vv, wpos, wpos);
			vwdview_s2w(vv, wpps, wpps);
			wpos[0] -= (float)vv->offset[0];
			wpos[1] -= (float)vv->offset[1];
			wpps[0] -= (float)vv->offset[0];
			wpps[1] -= (float)vv->offset[1];
			vv->ifdraw.motion(vv->data, wpos, wpps);
		}
		memcpy(vv->pps, e->motion, 3 * sizeof(float));
		break;
	case 3:
		if (wlezwrap_isclick(e->key[0])) {
			vwdview_mouse_click(vv, e->key[0], (bool)e->key[1]);
		} else {
			f_key(vv, e->key[0], (bool)e->key[1]);
		}
		break;
	}
}
