# set the compiler
CC := gcc

# set the compiler flags
CFLAGS :=  -ggdb3 -O0 -s -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# add header files here
HDRS := title.h constants.h

# add source files here
SRCS := main.c title.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := game

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)
install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${EXEC} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/snake

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
