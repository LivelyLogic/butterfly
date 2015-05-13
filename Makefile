CC = clang
CFLAGS = -Wall

QUARTZ_SOURCES = quartz/*.c
QUARTZ_HEADERS = quartz/*.h
QUARTZ_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(QUARTZ_SOURCES))))

LUA_SOURCES = lua/*.c
LUA_HEADERS = lua/*.h
LUA_OBJECTS = $(addsuffix .o, $(basename $(wildcard $(LUA_SOURCES))))

LIB = libbutterfly.a

quartz/%.o: quartz/%.c $(QUARTZ_HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

lua/%.o: lua/%.c $(LUA_HEADERS) $(QUARTZ_HEADERS)
	$(CC) -c -Iquartz -F/usr/local/include $(CFLAGS) $< -o $@

$(LIB): $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ar -cru $@ $(QUARTZ_OBJECTS) $(LUA_OBJECTS)
	ranlib $@

all: $(LIB)

clean:
	rm -f $(QUARTZ_OBJECTS) $(LUA_OBJECTS) $(LIB)
