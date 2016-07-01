##########################################################
# makefile for early-riser
##########################################################


# target
TARGET := bin/early-riser

# sources
SRCS := src/display_manager.c \
	src/time_domain.c \
	src/font.c \
	src/calendar.c \
	src/clock.c \
	src/alarm.c \
	src/disabler.c \
	src/main.c

# object files, dep files
OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.dep)
XDEPS := $(wildcard $(DEPS))

# compiler
CC = gcc

#
CCFLAGS = -std=gnu99 -Wall

#
CCFLAGS += -Wextra \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wnested-externs -Wmissing-include-dirs

#
LDFLAGS =

#
LIBS = -L/opt/vc/lib -lEGL -lGLESv2 -lbcm_host -lpthread -ljpeg -lshapes -lrt -lm

#
INCLUDE = -Iinclude \
	  -I/opt/vc/include \
	  -I/opt/vc/include/interface/vmcs_host/linux \
	  -I/opt/vc/include/interface/vcos/pthreads



#
all: dirs $(TARGET)

#
ifneq ($(XDEPS),)
include $(XDEPS)
endif

# directories
dirs::
	mkdir -p bin

#
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

#
$(OBJS): %.o: %.c %.dep
	$(CC) $(CCFLAGS) $(INCLUDE) -o $@ -c $<

#
$(DEPS): %.dep: %.c Makefile
	$(CC) $(CCFLAGS) $(INCLUDE) -MM $< > $@

# install to system
#install: all
#	cp $(TARGET) $(PSYNC_HOME)/bin/

#
clean:
	-rm -f src/*.o
	-rm -f src/*.dep
	-rm -f $(TARGET)
