#==============================================================================
# ■ makefile
#------------------------------------------------------------------------------
#   A placeholder for make
#==============================================================================

all:
	./make.rb VMDE
	./make.rb VMGS
clean:
	./make.rb clean
.PHONY: all clean
