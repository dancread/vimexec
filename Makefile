CC=cl /nologo
LINK=link
SOURCES=vimexec.c
CFLAGS=
OBJECTS=$(SOURCES:.c=.obj)
LIBS=kernel32.lib shell32.lib
all:
	$(CC) $(CFLAGS) $(SOURCES) /link $(LINK_FLAGS) $(LIBS)
clean:
	del $(OBJECTS) vimexec.exe
