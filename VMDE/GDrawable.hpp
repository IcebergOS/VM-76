//=============================================================================
// ■ GDrawable.hpp
//-----------------------------------------------------------------------------
//   GDrawable头文件。
//=============================================================================

#ifndef _INCLUDE_GDRAWBLE_H
#define _INCLUDE_GDRAWBLE_H

#include "global.hpp"

class GDrawable {
public:
	struct Data {
		GLfloat* vertices;
		int vtx_c;
		GLuint* indices;
		int ind_c;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint tri_mesh_count;
		glm::mat4 model;
	} data;

	void prepare(glm::mat4 projection, glm::mat4 view);
	void draw(GLuint start, GLuint end);
	void draw();
	void fbind();
	void update();
	void dispose();

	GDrawable();
};

#endif
