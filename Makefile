ROOTDIR		= $(shell pwd)
CC		= gcc
CFLAGS 		= -ggdb -Wall -pedantic -I$(ROOTDIR)/include $(shell pkg-config --cflags json-c) $(shell pkg-config --cflags libcurl)
LDFLAGS		= $(shell pkg-config --libs json-c)
LIB 		= $(ROOTDIR)/lib/LibreDTE.a
OBJECTS		= src/LibreDTE.o

all: $(LIB)

$(LIB): $(OBJECTS)
	mkdir -p `dirname $(LIB)`
	ar ru $@ $(OBJECTS)
	ranlib $@

clean:
	rm -rf $(ROOTDIR)/lib
	for OBJ in $(shell find | grep .o$$); do rm -f $$OBJ; done

.PHONY: ejemplos
ejemplos:
	cd ejemplos; $(CC) $(CFLAGS) 001-generar_dte.c -o 001-generar_dte $(LIB) -lcurl -ljson-c
	cd ejemplos; $(CC) $(CFLAGS) 002-actualizar_estado.c -o 002-actualizar_estado $(LIB) -lcurl
