GCC := /usr/local/gcc-13/bin/gcc
LLC := llc-18
LLI := lli-18
LINK := llvm-link-18
LX := ../../build/lx

BUILD := build
SRC := $(wildcard *.lx)
IR := $(patsubst %.lx,$(BUILD)/%.ll,$(SRC))
SINGLE := $(BUILD)/a.bc
OBJ := $(patsubst %.ll,%.o,$(IR))
TARGET := $(BUILD)/a

.PHONY: launch, interp

launch: $(TARGET)
	$(TARGET) > $(BUILD)/out.ppm

interp: $(SINGLE)
	$(LLI) $< > $(BUILD)/out.ppm

$(SINGLE): $(IR)
	$(LINK) -o $@ $^

$(TARGET): $(OBJ)
	$(GCC) -o $@ -lm $^

%.o: %.ll
	$(LLC) --filetype=obj -o $@ $<

$(BUILD)/%.ll: %.lx $(BUILD)
	$(LX) -o $@ $<

$(BUILD):
	mkdir -p $(BUILD)
