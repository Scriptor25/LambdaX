LX = ../../build/lx
LLC = llc
CC = gcc

SRCS = $(wildcard *.lx)
IRS = $(patsubst %.lx,%.ll,$(SRCS))
OBJS = $(patsubst %.ll,%.o,$(IRS))
TARGET = a.exe

ifeq ($(OS),Windows_NT)
	RM = del
else
	RM = rm
endif

.PHONY: clean build

clean:
	-$(RM) $(IRS) $(OBJS) $(TARGET)

build: $(TARGET)

%.ll: %.lx
	$(LX) -o $@ $<

%.o: %.ll
	$(LLC) -o $@ --filetype=obj $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ libglfw3.a -lm -lgdi32 -lopengl32
