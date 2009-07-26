!include <win32.mak>

CFLAGS = $(cflags) $(cdebug)
LDFLAGS = $(lflags) $(ldebug)
LDLIBS = glu.lib opengl.lib $(guilibsdll)

TARGETS = fullscreen.exe

default all: $(TARGETS)

# fullscreen requires glut.lib for fonts
fullscreen.exe: fullscreen.c
    $(cc) $(CFLAGS) -c $*.c
    $(link) $(LDFLAGS) -out:$@ $*.obj glut.lib $(LDLIBS)

clean:
    @del *.obj

clobber:
    @del *.obj
    @del *.exe


