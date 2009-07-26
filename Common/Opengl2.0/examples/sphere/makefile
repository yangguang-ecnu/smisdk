!include <win32.mak>

CFLAGS = $(cflags) $(cdebug)
LDFLAGS = $(lflags) $(ldebug)
LDLIBS = glu.lib opengl.lib $(guilibsdll)

TARGETS = sphere.exe 

default all: $(TARGETS)

# sphere requires winmm.lib for timing
sphere.exe: sphere.c
    $(cc) $(CFLAGS) -c $*.c
    $(link) $(LDFLAGS) -out:$@ $*.obj winmm.lib $(LDLIBS)

clean:
    @del *.obj

clobber:
    @del *.obj
    @del *.exe


