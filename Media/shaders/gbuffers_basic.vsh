#version 330 core
#pragma optimize(on)

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 3) in mat4 Model;

layout (std140) uniform Matrices {
	mat4 Projection;
	mat4 View;
	mat4 ProjectionView;
} matrices;

out vec4 texcolor;

void main() {
	gl_Position = matrices.ProjectionView * Model * vec4(position, 1.0);
	texcolor = vertex_color;
}
