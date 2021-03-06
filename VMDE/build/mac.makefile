#==============================================================================
# ■ VMDE - mac.makefile
#------------------------------------------------------------------------------
#   Mac does not support that make.rb, in a strange way...
#==============================================================================

TARGET = libVMDE.dylib

include ../inc.makefile
include ../routine.makefile

LDLIBS += $(shell pkg-config --libs glfw3 glm glew portaudio-2.0 ogg vorbisfile)
LDFLAGS += -dynamiclib -framework OpenGL
CXXFLAGS += -I../lib/lodepng -I.. -I../lib/ASM76 -c -fPIC \
	$(shell pkg-config --cflags glfw3 glm glew portaudio-2.0 vorbisfile)
