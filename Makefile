CC=cl /nologo
LINK=link
SOURCES=vimexec.c
CFLAGS=
OBJECTS=$(SOURCES:.c=.obj)
LIBS=kernel32.lib shell32.lib
all:
	$(CC) /DVIM_CONSOLE $(SOURCES) /link $(LINK_FLAGS) $(LIBS) /OUT:vimexec.exe  
	$(CC) /DVIM_GUI $(SOURCES) /link $(LINK_FLAGS) $(LIBS) /OUT:gvimexec.exe
clean:
	del $(OBJECTS) vimexec.exe
