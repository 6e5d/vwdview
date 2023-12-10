#include <cglm/cglm.h>
#include <wayland-client.h>

#include "../include/vwdview.h"

void vwdview_s2w(Vwdview *vv, vec2 s, vec2 w) {
	vec4 ss = {
		s[0] - (float)vv->window_size[0] * 0.5f,
		s[1] - (float)vv->window_size[1] * 0.5f,
		0.0f, 1.0f};
	vec4 ww;
	mat4 t;
	camcon2_build(&vv->camcon, t);
	glm_mat4_inv(t, t);
	glm_mat4_mulv(t, ss, ww);
	w[0] = ww[0] / ww[3]; w[1] = ww[1] / ww[3];
}

void vwdview_init(Vwdview* vv) {
	wlezwrap_confgen(&vv->wew);
	vv->wew.data = (void*)vv;
	vv->wew.hide_cursor = true;
	vv->wew.event = vwdview_event;
	wlezwrap_init(&vv->wew);
	camcon2_init(&vv->camcon);
	vv->camcon.k = 1.0;
	vv->resize = true;
	// just prevent initialize with 0 size
	vv->window_size[0] = 640;
	vv->window_size[1] = 480;
}

bool vwdview_flush_events(Vwdview* vv) {
	wl_display_roundtrip(vv->wew.wl.display);
	wl_display_dispatch_pending(vv->wew.wl.display);
	if (vv->resize) {
		printf("window size: %u %u\n",
			vv->window_size[0],
			vv->window_size[1]
		);
		vv->resize = false;
		return true;
	}
	return false;
}

void vwdview_deinit(Vwdview* vv) {
	wlezwrap_deinit(&vv->wew);
}

void vwdview_build_camera(Vwdview *vv, mat4 view) {
	camcon2_build(&vv->camcon, view);
}
