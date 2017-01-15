//=============================================================================
// ■ cube.hpp
//=============================================================================

#include <global.hpp>

#ifndef _INCLUDE_CUBES_H
#define _INCLUDE_CUBES_H

namespace VM76 {
	class Cube {
	public:
		GDrawable* obj = NULL;
		glm::mat4 *mat = NULL;

	private:
		GLfloat *vtx = NULL;
		GLuint *itx = NULL;

	public:
		Cube(int tid);
		void render();
		void update_instance(int mat_c, glm::mat4* mat);
		void dispose();
	};
}

#endif