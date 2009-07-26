!include <win32.mak>

CFLAGS = $(cflags) $(cdebug)
LDFLAGS = $(lflags) $(ldebug)
LDLIBS = glu.lib opengl.lib $(guilibsdll)

TARGETS = rb.exe

default all: $(TARGETS)

rb.exe: rb.c rb.res
    $(cc) $(CFLAGS) -c $*.c
    $(link) $(LDFLAGS) -out:$@ $*.obj $*.res winmm.lib $(LDLIBS)

.rc.res:
    $(rc) -r $<

clean:
    @del *.obj
    @del *.res

clobber:
    @del *.obj
    @del *.exe
    @del *.res


