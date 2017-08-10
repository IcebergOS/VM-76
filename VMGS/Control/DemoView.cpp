//=============================================================================
// ■ DemoView.cpp
//-----------------------------------------------------------------------------
//   Game view of a simple camera turnning, for demo
//=============================================================================

#include "control.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void DemoView::init_control(Camera* cam) {
		this->cam = cam;

		glfwSetInputMode(window, GLFW_STICKY_KEYS, 0);
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void DemoView::update_control() {
		float stime = float(VMDE->frame_count);
		float x = 5.0f * cos(stime * 0.015f);
		float z = 5.0f * sin(stime * 0.015f);
		cam->wpos = glm::vec3(x, 3.5f, z);
		cam->rotate = glm::vec3(0.5, -PI * 0.5 - stime * 0.015f, 0.0);
	}
}
