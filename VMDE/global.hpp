//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   VMDE通用C++头文件。
//=============================================================================

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ruby.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#ifndef _INCLUDE_GLOBAL_H
#define _INCLUDE_GLOBAL_H

	typedef VALUE (*type_ruby_function)(ANYARGS);

	// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	#ifdef __CYGWIN__
		#warning This wont work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif

	// Global
	extern const char* GAME_NAME;

	struct VMDEState {
		bool frozen;
	};

	struct VMDE {
		struct VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
	};

	struct RenderChainNode {
		struct RenderChainNode* prev;
		VALUE n;
		struct RenderChainNode* next;
	};

	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern VALUE Global_module;
	extern VALUE GResPic;
	extern struct RenderChainNode* render_chain;

	extern GLuint VBO[15];
	extern GLuint VAO[15];

	// shaders.cpp
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;
		GLuint shaderProgram;

	public:
		void init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		void link_program();
	};

	extern const GLchar* temp_vertexShaderSource;
	extern const GLchar* temp_fragmentShaderSource;

	extern Shaders* main_shader;

	// ruby_connection.cpp
	namespace RubyWrapper {
		VALUE load_pic(VALUE self, VALUE path);
		VALUE init_engine(VALUE self, VALUE w, VALUE h);
		VALUE main_draw_loop();
		VALUE main_get_frame_count();
		VALUE main_get_fps();
	}
	void init_ruby_classes();
	void init_ruby_modules();

	// init.cpp
	void init_engine(int w, int h);
	void setup_viewport();

	// main.cpp
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();

	void matrix2D();

	// resources.cpp
	int load_img(char* p);

	// util.cpp
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		void log_internal(const char* function_name, const char* format, ...);
	}

#endif
