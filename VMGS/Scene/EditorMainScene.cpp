//=============================================================================
// ■ VMGS/Scene/EditorMainScene.cpp
//-----------------------------------------------------------------------------
//   编辑器场景。
//=============================================================================

#include "../VMGS.hpp"

#include "../Control/control.hpp"
#include "../GameObject/SkyBox/SkyBox.hpp"

namespace VM76 {
	// There can be multiple controls.
	size_t ctl_count, ctl_index;
	Control* ctls[16] = {NULL};

	SkyBox* sky;

	GDrawable* hand_block;

	//-------------------------------------------------------------------------
	// ● 缓冲区设定
	//-------------------------------------------------------------------------
	enum BuffersIndex {
		BufferAlbedo,
		BufferNormal,
		BufferLighting,
		BufferComposite
	};

	//-------------------------------------------------------------------------
	// ● 场景开始
	//-------------------------------------------------------------------------
	EditorMainScene::EditorMainScene() {
		obj = new GObject();
		physics = new PhyEngine();

		hand_block = new GDrawable(NULL, NULL);
		hand_block->data.vertices = (GLuint*)new Vertex[4 * 6];
		hand_block->data.indices = new GLuint[6 * 6];

		shader_textured.add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_textured.vsh");
		shader_textured.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_textured.fsh");
		shader_textured.link_program();
		shader_basic.add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_basic.vsh");
		shader_basic.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_basic.fsh");
		shader_basic.link_program();
		gui.add_file(GL_VERTEX_SHADER, "../Media/shaders/gui.vsh");
		gui.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gui.fsh");
		gui.link_program();
		final_composite.add_file(GL_VERTEX_SHADER, "../Media/shaders/composite.vsh");
		final_composite.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/composite.fsh");
		final_composite.link_program();
		deferred_lighting.add_file(GL_VERTEX_SHADER, "../Media/shaders/deferred_lighting.vsh");
		deferred_lighting.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/deferred_lighting.fsh");
		deferred_lighting.link_program();
		deferred_composite.add_file(GL_VERTEX_SHADER, "../Media/shaders/composite_deferred.vsh");
		deferred_composite.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/composite_deferred.fsh");
		deferred_composite.link_program();

		GLuint* gbuffers_type = new GLuint[4]{GL_RGB8, GL_RGB8, GL_RGB8, GL_RGB16F};
		// Albedo, Normal, Lighting, Composite
		postBuffer = new RenderBuffer(VMDE->width, VMDE->height, 4, gbuffers_type);

		// Set up hand block indicator's matrix
		glm::mat4 block_display = glm::translate(
			glm::mat4(1.0),
			glm::vec3(0.02, 0.06, 0.2)
		);
		block_display = glm::scale(block_display, glm::vec3(0.1f));
		block_display = glm::rotate(block_display,
			PIf / 4.0f,
			glm::vec3(1.0, 0.0, 0.0)
		);
		block_display = glm::rotate(block_display,
			PIf / 4.0f,
			glm::vec3(0.0, 1.0, 0.0)
		);

		TiledMap::init_cinstances(clist);
		int vtx_c = 0, ind_c = 0;
		for (int i = 0; i < 6; i++) clist[hand_id - 1]->bake(0,0,0,(Vertex*)hand_block->data.vertices,hand_block->data.indices,&vtx_c,&ind_c,i);
		hand_block->data.vtx_c = vtx_c;
		hand_block->data.ind_c = ind_c;
		hand_block->data.mat_c = 1;
		hand_block->data.mat = (GLuint*) &block_display;
		hand_block->fbind();
		block_pointer.obj->data.mat_c = 1;


		cam = new Camera(glm::vec3(0.0), glm::vec3(0.0), glm::perspective(1.3f, aspect_ratio, 0.1f, 1000.0f));

		ctl_index = 2; // initially FirstPersonView
		ctl_count = 3;
		DemoView* ctl0 = new DemoView();
		ctl0->init_control(cam);
		ctls[0] = ctl0;
		GodView* ctl1 = new GodView();
		ctl1->init_control(cam);
		ctl1->cam->wpos = glm::vec3(64.0, 72.0, 64.0);
		ctls[1] = ctl1;
		FirstPersonView* ctl2 = new FirstPersonView();
		ctl2->init_control(cam);
		ctl2->game_player.wpos = glm::vec3(64.0, 72.0, 64.0);
		ctls[2] = ctl2;

		sky = new SkyBox({
			"../Media/skybox/skybox_0.png",
			"../Media/skybox/skybox_1.png",
			"../Media/skybox/skybox_2.png",
			"../Media/skybox/skybox_3.png",
			"../Media/skybox/skybox_4.png",
			"../Media/skybox/skybox_5.png"
		}, cam);

		phy_map = new PhysicsMap(&map);
		physics->add_obj(phy_map);
	}
	//-------------------------------------------------------------------------
	// ● 按键回调
	//-------------------------------------------------------------------------
	bool magnify = false;
	bool magnifyPrev = false;

	void EditorMainScene::key_callback(int key, int scancode, int action, int mods) {
		#define PRESS(n) key == (n) && action == GLFW_PRESS
		if (PRESS(GLFW_KEY_F5)) {
			if (mods & GLFW_MOD_SHIFT) {
				ctl_index--;
				if (ctl_index == SIZE_MAX) ctl_index = ctl_count - 1;
			} else {
				ctl_index++;
				if (ctl_index >= ctl_count) ctl_index = 0;
			}
		}

		if (PRESS(GLFW_KEY_0)) hand_id = 0;
		if (PRESS(GLFW_KEY_1)) hand_id = 1;
		if (PRESS(GLFW_KEY_2)) hand_id = 2;
		if (PRESS(GLFW_KEY_3)) hand_id = 3;
		if (PRESS(GLFW_KEY_4)) hand_id = 4;
		if (PRESS(GLFW_KEY_5)) hand_id = 5;
		if (PRESS(GLFW_KEY_6)) hand_id = 6;
		if (PRESS(GLFW_KEY_7)) hand_id = 7;
		if (PRESS(GLFW_KEY_8)) hand_id = 8;
		if (PRESS(GLFW_KEY_9)) hand_id = 9;

		if (hand_id > 0) {
			int vtx_c = 0, ind_c = 0;
			for (int i = 0; i < 6; i++) {
				clist[hand_id - 1]->bake(
					0, 0, 0,
					(Vertex*) hand_block->data.vertices,
					hand_block->data.indices,
					&vtx_c, &ind_c,
					i
				);
			}
			hand_block->data.vtx_c = vtx_c;
			hand_block->data.ind_c = ind_c;
			hand_block->data.mat_c = 1;
			hand_block->update();
		}

		if (PRESS(GLFW_KEY_R)) {
			map.place_block(obj->pos, hand_id);
		}

		if (key == GLFW_KEY_O) {
			if (action == GLFW_PRESS) {
				cam->Projection = glm::perspective(0.52f, aspect_ratio, 0.1f, 1000.0f);
			} else if (action == GLFW_RELEASE) {
				cam->Projection = glm::perspective(1.2f, aspect_ratio, 0.1f, 1000.0f);
			}
		}

		if (PRESS(GLFW_KEY_Z)) {
			log("Saving map");
			map.map->save_map();
		}

		static Audio::Channel_Vorbis* loop = NULL;
		static Audio::Channel_Triangle* triangle = NULL;
		static Audio::Channel_Sine* sine = NULL;
		if (PRESS(GLFW_KEY_SEMICOLON)) {
			Audio::play_sound("../Media/soft-ping.ogg", false);
		}
		if (PRESS(GLFW_KEY_APOSTROPHE)) {
			if (loop) {
				Audio::stop(loop);
				loop = NULL;
			} else {
				loop = Audio::play_sound("../Media/loop-test.ogg", true, .1f);
			}
		}
		if (PRESS(GLFW_KEY_LEFT_BRACKET)) {
			if (triangle) {
				Audio::stop(triangle);
				triangle = NULL;
			} else {
				triangle = new Audio::Channel_Triangle(440);
				Audio::play_channel(triangle);
			}
		}
		if (PRESS(GLFW_KEY_RIGHT_BRACKET)) {
			if (sine) {
				Audio::stop(sine);
				sine = NULL;
			} else {
				sine = new Audio::Channel_Sine(440);
				Audio::play_channel(sine);
			}
		}
		#undef PRESS
	}
	//-------------------------------------------------------------------------
	// ● 刷新
	//-------------------------------------------------------------------------
	void EditorMainScene::update() {
		// Pick
		//  暂时只有拣选地图Tile功能，其它的拣选可以参考RM的分layer拣选
		glm::mat3 inverse_view = glm::inverse(glm::mat3(cam->View));
		glm::vec3 pos = glm::normalize(inverse_view * glm::vec3(0.0, 0.0, -1.0));
		glm::vec3 test = cam->wpos;

		for (int i = 0; i < 32; i++) {
			if (Tiles::is_valid(map.map->tidQuery(test.x, test.y, test.z))) {
				// we got something...
				test = test - glm::fract(test);
				obj->pos = test;
				break;
			}

			test += pos * 0.3f;
		}

		// Update other stuff
		ctls[ctl_index]->update_control();
		physics->tick();
		cam->update();
	}
	//-------------------------------------------------------------------------
	// ● 渲染
	//-------------------------------------------------------------------------
	void EditorMainScene::render() {
		shader_textured.use();

		// Setup uniforms
		shader_textured.set_float("brightness", VMDE->state.brightness);
		shader_textured.set_texture("colortex0", &tile_texture, 0);

		// ================ STAGE 1 ================
		//  Object rendering Opaque & cut-outs
		//  Bind Post buffer & use stencil
		postBuffer->bind();
		RenderBuffer::clearBuffer(glm::vec4(0.5, 0.7, 1.0, 0.0), true, true, true);
		postBuffer->set_draw_buffers();

		// setup mask
		glStencilFunc(GL_ALWAYS, 1, 0x01);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		// Textured blocks rendering
		shader_textured.ProjectionView(cam->Projection, cam->View);
		map.render();

		// Setup uniforms
		// Non textured rendering
		shader_basic.use();
		shader_basic.set_float("opaque", 0.4 + 0.2 * glm::sin(VMDE->frame_count * 0.1));
		shader_basic.ProjectionView(cam->Projection, cam->View);
		block_pointer.mat[0] = obj->transform();
		block_pointer.update_instance(1);
		block_pointer.render();

		axe.render();

		// ================ STAGE 2 ================
		//  Deferred shading Opaque & cut-outs
		//  Read Post buffer & stencil mask
		glStencilFunc(GL_EQUAL, 1, 0x01);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0x00);

		deferred_lighting.use();
		deferred_lighting.set_texture("normal", postBuffer->texture_buffer[BufferNormal], 1);
		glm::vec3 sunVec = glm::mat3(cam->View) * glm::vec3(cos(PIf * 0.25), sin(PIf * 0.25), sin(PIf * 0.25) * 0.3f);
		deferred_lighting.set_vec3("sunVec", sunVec);
		PostProcessingManager::Blit2D();

		// Combine lighting with albedo
		deferred_composite.use();
		deferred_composite.set_texture("albedo", postBuffer->texture_buffer[BufferAlbedo], 0);
		deferred_composite.set_texture("lighting", postBuffer->texture_buffer[BufferLighting], 1);
		deferred_composite.set_texture("depthtex", postBuffer->texture_buffer[4], 14);
		deferred_composite.set_vec2("clipping", glm::vec2(0.1, 1000.0));
		deferred_composite.set_vec3("fogColor", glm::vec3(0.5, 0.7, 1.0));
		PostProcessingManager::Blit2D();

		// ================ STAGE 3 ================
		//  Skybox (Unfilled area) shading
		//  Reverse stencil mask

		glStencilFunc(GL_NOTEQUAL, 1, 0x01);
		VMStateControl::disable_depth_test();
		sky->render();

		// ================ STAGE 4 ================
		//  Final composite & Full screen shading
		VMStateControl::disable_stencil_test();

		postBuffer->unbind();
		final_composite.use();
		final_composite.set_texture("composite", postBuffer->texture_buffer[BufferComposite], 15);
		PostProcessingManager::Blit2D();

		// ================ STAGE 5 ================
		//  GUI rendering
		gui.use();
		gui.set_texture("atlastex", &tile_texture, 0);
		gui.ProjectionView(gui_2d_projection, glm::mat4(1.0));
		VMSC::disable_depth_test();
		if (hand_id > 0) hand_block->renderOnce();

		if (SceneManager::render_debug_info) {
			char info[64];
			sprintf(info, "Hand ID: %d  Pointer ID: %d",
				hand_id,
				map.map->tidQuery(obj->pos.x, obj->pos.y, obj->pos.z)
			);
			trex->instanceRenderText(
				info, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.88, 0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
			sprintf(info, "Pointer pos: (%.0f, %.0f, %.0f)",
				obj->pos.x, obj->pos.y, obj->pos.z
			);
			trex->instanceRenderText(
				info, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.82, 0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
		}
		VMSC::enable_depth_test();
	}
	//-------------------------------------------------------------------------
	// ● 释放
	//-------------------------------------------------------------------------
	EditorMainScene::~EditorMainScene() {
		XE(delete, phy_map);
		XE(delete, physics);

		for (int i = 0; i < 16; i++) XE(delete, clist[i]);
		XE(delete, trex);
		XE(delete, postBuffer);
	}
}
