CC = clang
CFLAGS = -Wall
INSTALL = install -p -m 0644

INSTALL_LIB = /usr/local/lib
LUA_LIB = /usr/local/lib/liblua.a
LUA_INCLUDE = /usr/local/include

QUARTZ_SOURCES = quartz/*.c
QUARTZ_HEADERS = quartz/*.h
QUARTZ_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(QUARTZ_SOURCES))))

LUA_SOURCES = lua/*.c
LUA_HEADERS = lua/*.h
LUA_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(LUA_SOURCES))))

LUA2PNG_SOURCE = lua2png.c
LUA2PNG_OBJECT = lua2png.o
LUA2PNG_LIBS = $(LUA_LIB)
LUA2PNG_FRAMEWORKS = -framework CoreFoundation -framework CoreGraphics -framework CoreText -framework ImageIO

LIB = libbutterfly.a

quartz/%.o: quartz/%.c $(QUARTZ_HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

lua/%.o: lua/%.c $(LUA_HEADERS) $(QUARTZ_HEADERS)
	$(CC) -c -Iquartz -F$(LUA_INCLUDE) $(CFLAGS) $< -o $@

$(LUA2PNG_OBJECT): $(LUA2PNG_SOURCE) $(LUA_HEADERS) $(QUARTZ_HEADERS)
	$(CC) -c -Iquartz -Ilua -F$(LUA_INCLUDE) $(CFLAGS) $< -o $@

$(LIB): $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ar -cru $@ $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ranlib $@

all: $(LIB)

clean:
	rm -f $(QUARTZ_OBJECTS) $(LUA_OBJECTS) $(LIB)
	rm -f $(LUA2PNG_OBJECT) lua2png

install: $(LIB)
	$(INSTALL) $(LIB) $(INSTALL_LIB)

lua2png: $(LUA2PNG_OBJECT) $(LIB)
	$(CC) -o $@ $(LUA2PNG_OBJECT) $(LIB) $(LUA2PNG_LIBS) $(LUA2PNG_FRAMEWORKS)