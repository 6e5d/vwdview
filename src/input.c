#include <cglm/cglm.h>
#include <stdint.h>

#include "../../wlezwrap/include/wlezwrap.h"
#include "../../wlezwrap/include/mview.h"
#include "../include/input.h"
#include "../include/vwdview.h"

static double angle_norm(double angle) {
	while (angle >= M_PI) {
		angle -= 2.0 * M_PI;
	}
	while (angle < M_PI) {
		angle += 2.0 * M_PI;
	}
	return angle;
}

void vwdview_mview_event(WlezwrapMview *wewmv, double x, double y) {
	Vwdview *iv = (Vwdview*)wewmv->data;
	if (wewmv->button == 0) {
		vec2 wpos = {(float)(x - wewmv->px), (float)(y - wewmv->py)};
		camcon2_s2w(&iv->camcon, wpos, wpos);
		iv->camcon.x += wpos[0];
		iv->camcon.y += wpos[1];
	} else if (wewmv->button == 1) {
		double cx = (double)iv->window_size[0] / 2.0;
		double cy = (double)iv->window_size[1] / 2.0;
		double t0 = atan2(wewmv->py - cy, wewmv->px - cx);
		double t1 = atan2(y - cy, x - cx);
		t1 = angle_norm(t1 - t0);
		iv->camcon.theta += (float)t1;
	} else if (wewmv->button == 2) {
		iv->camcon.k *=  1.0f - (float)(y - wewmv->py) * 0.005f;
	}
}

static void f_key(Vwdview *iv, char key, bool pressed) {
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

void vwdview_event(void* data, uint8_t type, WlezwrapEvent *e) {
	Vwdview* vv = data;
	wlezwrap_mview_update(&vv->wewmv, type, e);
	switch(type) {
	case 0:
		f_quit(vv);
		break;
	case 1:
		f_resize(vv, e->resize[0], e->resize[1]);
		break;
	case 3:
		f_key(vv, e->key[0], (bool)e->key[1]);
		break;
	}
	if (vv->event != NULL) {
		vv->event(vv, type, e);
	}
}
