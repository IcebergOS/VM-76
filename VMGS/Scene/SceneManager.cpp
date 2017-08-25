//=============================================================================
// ■ VMGS/Scene/SceneManager.cpp
//-----------------------------------------------------------------------------
//   VMGS的场景管理器，负责场景的创建、转移、清理等工作。
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 { namespace SceneManager {
	//-------------------------------------------------------------------------
	// ● 模块变量
	//-------------------------------------------------------------------------
	// 当前场景的实例
	Scene* scene = NULL;
	// 在所有场景都会用到的调试信息的开关
	bool debug_info_visible = true;

	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		glfwSetKeyCallback(window, SceneManager::key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
	}

	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void update_scene() {
		if (scene) scene->update();
	}

	//-------------------------------------------------------------------------
	// ● 绘制
	//-------------------------------------------------------------------------
	void render_scene() {
		if (scene) scene->render();
		render_debug_info();
	}

	//-------------------------------------------------------------------------
	// ● 绘制调试信息
	//-------------------------------------------------------------------------
	void render_debug_info() {
		if (debug_info_visible) {
			VMSC::disable_depth_test();
			char fps[40];
			sprintf(fps, "FPS: %d, %f.4ms", VMDE->fps, VMDE->frame_time);
			TextRenderer::BakeOptions opt = {
				.text = fps,
				.width = 0.025,
				.height = 0.05,
				.decoration = TextRenderer::TextDecorationType::OUTLINE,
				.color = glm::vec4(1.0, 1.0, 1.0, 1.0),
			};
			trex->instanceRenderText(
				&opt,
				gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.94, 0.0))
			);
			VMSC::enable_depth_test();
		}
	}

	//-------------------------------------------------------------------------
	// ● 供GLFW使用的按键回调
	//-------------------------------------------------------------------------
	void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
		if (w != window) return;
		if (scene) {
			scene->event_key(key, action);
			if (action == GLFW_PRESS) {
				scene->event_keydown(key, mods);
			}
		}
	}
	//-------------------------------------------------------------------------
	// ● 供GLFW使用的鼠标键回调
	//-------------------------------------------------------------------------
	void mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
		if (w != window) return;
		if (scene) {
			scene->event_mousebutton(button, action, mods);
		}
	}
}}