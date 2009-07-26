!include <win32.mak>

CFLAGS = $(cflags) $(cdebug)
LDFLAGS = $(lflags) $(ldebug)
LDLIBS = glu.lib opengl.lib $(guilibsdll)

TARGETS = \
	cube.exe \
	indexfnc.exe \
	indexmat.exe \
	indextex.exe \
	sdib.exe \
	simpleci.exe \
	texenv.exe \
	torus.exe \
	vcull.exe \
	wglinfo.exe

default all: $(TARGETS)

.c.exe:
    $(cc) $(CFLAGS) -c $*.c
    $(link) $(LDFLAGS) -out:$@ $*.obj $(LDLIBS)

clean:
    @del *.obj

clobber:
    @del *.obj
    @del *.exe


