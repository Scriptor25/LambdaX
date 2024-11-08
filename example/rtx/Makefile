GCC := gcc
LLC := llc
LLI := lli
LINK := llvm-link
LX := lx

BUILD := build
SRC := $(wildcard *.lx)
IR := $(patsubst %.lx,$(BUILD)/%.ll,$(SRC))
SINGLE := $(BUILD)/a.bc
OBJ := $(patsubst %.ll,%.o,$(IR))
TARGET := $(BUILD)/a.exe

.PHONY: launch, interp

launch: $(TARGET)
	$(TARGET) > $(BUILD)/out.ppm

interp: $(SINGLE)
	$(LLI) $< > $(BUILD)/out.ppm

$(SINGLE): $(IR)
	$(LINK) -o $@ $^

$(TARGET): $(OBJ)
	$(GCC) -o $@ $^

%.o: %.ll
	$(LLC) --filetype=obj -o $@ $<

$(BUILD)/%.ll: %.lx
	$(LX) -o $@ $<
