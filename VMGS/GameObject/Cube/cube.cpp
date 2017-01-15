//=============================================================================
// ■ cube.cpp
//-----------------------------------------------------------------------------
//   3D模型：方块
//=============================================================================

#include "cube.hpp"

namespace VM76 {
	Cube::Cube(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx = new GLfloat[9 * 4 * 6] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,

			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,

			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,

			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,

			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,

			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + S,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + S,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs + T, ys + T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs + S, ys + T,
		};
		itx = new GLuint[6 * 6] {
			0 + 0,  0 + 1,  0 + 3,   0 + 1,  0 + 2,  0 + 3,
			4 + 3,  4 + 1,  4 + 0,   4 + 3,  4 + 2,  4 + 1,
			8 + 0,  8 + 1,  8 + 3,   8 + 1,  8 + 2,  8 + 3,
			12 + 3, 12 + 1, 12 + 0,  12 + 3, 12 + 2, 12 + 1,
			16 + 0, 16 + 1, 16 + 3,  16 + 1, 16 + 2, 16 + 3,
			20 + 3, 20 + 1, 20 + 0,  20 + 3, 20 + 2, 20 + 1,
		};

		// Prepare an empty space
		glm::mat4* mat = new glm::mat4[512];
		for (int x = 0; x < 512; x++) mat[x] = glm::mat4(1.0);

		obj = new GDrawable();
		obj->data.vtx_c = 9 * 4 * 6;
		obj->data.ind_c = 2 * 3 * 6;
		obj->data.vertices = vtx;
		obj->data.indices = itx;
		obj->data.tri_mesh_count = 2 * 6;
		// Reserve 512 spaces
		obj->data.mat_c = 512;
		obj->data.mat = (GLuint*) &mat[0];
		obj->fbind();
		// Draw none for default
		obj->data.mat_c = 0;

		mat[1] = glm::translate(glm::mat4(1.0), glm::vec3(1.0));
		update_instance(2, &mat[0]);

		xefree(mat);
	}

	void Cube::update_instance(int mat_c, glm::mat4* mat) {
		obj->data.mat_c = mat_c;
		obj->data.mat = (GLuint*) mat;
		obj->update_instance();
	}

	void Cube::render() {
		obj->draw();
	}

	void Cube::dispose() {
		obj->dispose();
		xefree(vtx); xefree(itx);
	}
}