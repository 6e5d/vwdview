#include <cglm/cglm.h>
#include <math.h>

#include "../../wlezwrap/include/wlezwrap.h"
#include "../include/vwdview.h"

const static float math_pi = 3.1415926536f;

static float angle_norm(float angle) {
	while (angle >= math_pi) {
		angle -= 2.0f * math_pi;
	}
	while (angle < math_pi) {
		angle += 2.0f * math_pi;
	}
	return angle;
}

static void f_key(Vwdview() *iv, uint8_t key, bool pressed) {
	if (!pressed) {
		iv->cb_key(iv->data, key, pressed);
		return;
	}
	if (key == 'i') {
		iv->camcon.k *= 1.33f;
	} else if (key == 'o') {
		iv->camcon.k /= 1.33f;
	} else if (key == 'z') {
		iv->cb_undo(iv->data, true);
	} else if (key == 'y') {
		iv->cb_undo(iv->data, false);
	} else if (key == 'r') {
		iv->camcon.theta = 0.0f;
		iv->camcon.mirror = false;
	} else if (key == 'm') {
		iv->camcon.mirror = !iv->camcon.mirror;
	} else {
		iv->cb_key(iv->data, key, pressed);
	}
}

static void f_resize(Vwdview() *vv, uint32_t w, uint32_t h) {
	vv->window_size[0] = w;
	vv->window_size[1] = h;
	vv->resize = true;
}

static void f_quit(Vwdview() *vv) {
	vv->quit = true;
}

static void vwdview(mouse_click)(Vwdview()* vv, uint8_t id, bool updown) {
	bool *keystate = vv->wew.keystate;
	if (updown) {
		if (keystate[wlezwrap(lshift)]) {
			vv->input_state = 2;
		} else if (keystate[wlezwrap(lctrl)]) {
			vv->input_state = 3;
		} else if (keystate[' '] || id == wlezwrap(mclick)) {
			vv->input_state = 1;
		} else if (keystate[wlezwrap(lalt)]) {
			vv->input_state = 5;
		} else if (id == wlezwrap(lclick)) {
			vv->input_state = 4;
		}
		vv->skip = true;
	} else {
		if (id == wlezwrap(lclick)) {
			vv->ifdraw.end(vv->brush);
			vv->cb_submit(vv->data);
		}
		vv->input_state = 0;
	}
}

static void vwdview(view_pan)(Vwdview()* vv, Wlezwrap(Event) *e) {
	vec2 wpos, wpps;
	vwdview(s2w)(vv, e->motion, wpos);
	vwdview(s2w)(vv, vv->pps, wpps);
	vv->camcon.x += wpos[0] - wpps[0];
	vv->camcon.y += wpos[1] - wpps[1];
}

static void vwdview(view_rotate)(Vwdview()* vv, Wlezwrap(Event) *e) {
	float cx = (float)vv->window_size[0] / 2.0f;
	float cy = (float)vv->window_size[1] / 2.0f;
	float t0 = atan2f(vv->pps[1] - cy, vv->pps[0] - cx);
	float t1 = atan2f(e->motion[1] - cy, e->motion[0] - cx);
	t1 = angle_norm(t1 - t0);
	camcon2(rotate_view)(&vv->camcon, (float)t1);
}

static void vwdview(view_zoom)(Vwdview()* vv, Wlezwrap(Event) *e) {
	vv->camcon.k *=  1.0f - (float)(e->motion[1] - vv->pps[1]) * 0.005f;
}

void vwdview(event)(void* data, uint8_t type, Wlezwrap(Event) *e) {
	Vwdview()* vv = data;
	if (type == 0) {
		f_quit(vv);
	} else if (type == 1) {
		f_resize(vv, e->resize[0], e->resize[1]);
	} else if (type == 2) {
		if (vv->skip) {
			vv->skip = false;
		} else if (vv->input_state == 1) {
			vwdview(view_pan)(vv, e);
		} else if (vv->input_state == 2) {
			vwdview(view_rotate)(vv, e);
		} else if (vv->input_state == 3) {
			vwdview(view_zoom)(vv, e);
		} else if (vv->input_state == 4) {
			float wpos[3], wpps[3];
			memcpy(wpos, &e->motion, 3 * sizeof(float));
			memcpy(wpps, &vv->pps, 3 * sizeof(float));
			vwdview(s2w)(vv, wpos, wpos);
			vwdview(s2w)(vv, wpps, wpps);
			wpos[0] -= (float)vv->offset[0];
			wpos[1] -= (float)vv->offset[1];
			wpps[0] -= (float)vv->offset[0];
			wpps[1] -= (float)vv->offset[1];
			vv->ifdraw.motion(vv->brush, wpos, wpps);
		} else if (vv->input_state == 5) {
			float dx = e->motion[0] - vv->pps[0];
			vv->ifdraw.primary(vv->brush, dx);
		}
		memcpy(vv->pps, e->motion, 3 * sizeof(float));
	} else if (type == 3) {
		if (wlezwrap(isclick)(e->key[0])) {
			vwdview(mouse_click)(vv, e->key[0], (bool)e->key[1]);
		} else {
			f_key(vv, e->key[0], (bool)e->key[1]);
		}
	}
}
