CC = g++
AR = ar
PREFIX=..

# Uncomment one of the following to switch between debug and opt mode
#OPT = -O3 -DNDEBUG
OPT = -g -ggdb

CFLAGS += --std=c++11 -fno-strict-aliasing -Wall -c -I. -I./include -I./src/ -I"C:\Program Files\OpenSSL-Win64\include" $(OPT)

#LDFLAGS+= -Wall -lpthread -lssl -lcrypto
LDFLAGS+= -Wall -lpthread -L"C:\Program Files\OpenSSL-Win64\lib" -lssl -lcrypto

LIBOBJECTS = \
	./src/hashutil.o \

HEADERS = $(wildcard src/*.h)
ALIB = libcuckoofilter.a

TEST = test

all: $(TEST)

clean:
	rm -f $(TEST) */*.o

test: example/test.o $(LIBOBJECTS) 
	$(CC) example/test.o $(LIBOBJECTS) $(LDFLAGS) -o $@

%.o: %.cc ${HEADERS} Makefile
	$(CC) $(CFLAGS) $< -o $@

$(ALIB): $(LIBOBJECTS)
	$(AR) rcs $@ $(LIBOBJECTS)

.PHONY: install
install: $(ALIB)
#install -D -m 0755 $(HEADERS) -t $(DESTDIR)$(PREFIX)/include/cuckoofilter
	install -D -m 0755 $(HEADERS) -t include/cuckoofilter
	install -D -m 0755 $< -t lib

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(ALIB)
	rm -rf $(DESTDIR)$(PREFIX)/include/cuckoofilter
