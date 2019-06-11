CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDLIBS = -lpthread

ifdef DEBUG
CFLAGS += -g
LDFLAGS += -g
endif

TARGETS =  libthreadpool.so libthreadpool.a

all: $(TARGETS)

src/threadpool.o: src/threadpool.c src/threadpool.h


# Short-hand aliases
shared: libthreadpool.so
static: libthreadpool.a

libthreadpool.so: src/threadpool.c src/threadpool.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

src/libthreadpool.o: src/threadpool.c src/threadpool.h
	$(CC) -c -fPIC ${CFLAGS} -o $@ $<

libthreadpool.a: src/libthreadpool.o
	ar rcs $@ $^

clean:
	rm -f $(TARGETS) *~ */*~ */*.o

