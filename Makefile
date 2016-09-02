CC = clang
CFLAGS = -Wall
INSTALL_DIR = install -d
INSTALL = install -p -m 0644

INSTALL_LIB = /usr/local/lib
INSTALL_HEADER = /usr/local/include/butterfly
LUA_INCLUDE = /usr/local/include

QUARTZ_SOURCES = quartz/*.c
QUARTZ_HEADERS = quartz/*.h
QUARTZ_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(QUARTZ_SOURCES))))

LUA_SOURCES = lua/*.c
LUA_HEADERS = lua/*.h
LUA_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(LUA_SOURCES))))

LUA2PNG_SOURCE = lua2png.c
LUA2PNG_OBJECT = lua2png.o
LUA2PNG_LIBS = -L$(INSTALL_LIB) -llua -lbutterfly
LUA2PNG_FRAMEWORKS = -framework CoreFoundation -framework CoreGraphics -framework CoreText -framework ImageIO

LIB = libbutterfly.a
HEADER = lua/lua.h quartz/butterfly.h quartz/quartz.h

quartz/%.o: quartz/%.c $(QUARTZ_HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

lua/%.o: lua/%.c $(LUA_HEADERS) $(QUARTZ_HEADERS)
	$(CC) -c -Iquartz -I$(LUA_INCLUDE) $(CFLAGS) $< -o $@

$(LUA2PNG_OBJECT): $(LUA2PNG_SOURCE) $(LUA_HEADERS) $(QUARTZ_HEADERS)
	$(CC) -c -I$(LUA_INCLUDE) $(CFLAGS) $< -o $@

$(LIB): $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ar -cru $@ $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ranlib $@

all: $(LIB)

clean:
	rm -f $(QUARTZ_OBJECTS) $(LUA_OBJECTS) $(LIB)
	rm -f $(LUA2PNG_OBJECT) lua2png

install: $(LIB) $(HEADER)
	$(INSTALL) $(LIB) $(INSTALL_LIB)
	$(INSTALL_DIR) $(INSTALL_HEADER)
	$(INSTALL) $(HEADER) $(INSTALL_HEADER)

lua2png: $(LUA2PNG_OBJECT)
	$(CC) -o $@ $(LUA2PNG_OBJECT) $(LUA2PNG_LIBS) $(LUA2PNG_FRAMEWORKS)
