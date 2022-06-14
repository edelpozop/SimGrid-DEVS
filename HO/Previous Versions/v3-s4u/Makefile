BIN_FILES  =  main

INSTALL_PATH = /opt/simgrid-v3.29p

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -I/usr/local/include/


NO_PRAYER_FOR_THE_WICKED = -w -O3

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lm -lsimgrid -rdynamic $(INSTALL_PATH)/lib/libsimgrid.so -Wl,-rpath,$(INSTALL_PATH)/lib


all: CFLAGS=$(NO_PRAYER_FOR_THE_WICKED)
all: $(BIN_FILES)
.PHONY : all

main: main.o sim.o
	$(CC) -pg $(LDFLAGS) $^ $(LDLIBS) -o $@


%.o: %.c
	$(CC) -pg $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
