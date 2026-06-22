# Placeholder Makefile for OPEN-R object development.
# Replace compiler/linker variables with the values used by your installed SDK samples.

OPENRSDK_ROOT ?= /usr/OPENR_SDK
BUILD_DIR := build
TARGET := $(BUILD_DIR)/HelloBody.bin

.PHONY: all clean explain

all: explain

explain:
	@mkdir -p $(BUILD_DIR)
	@echo "This is a repo scaffold."
	@echo "Use a known-working OPEN-R SDK sample Makefile as the base, then point it at HelloBody.cc."
	@echo "Expected SDK root: $(OPENRSDK_ROOT)"
	@echo "Target placeholder: $(TARGET)"

clean:
	rm -rf $(BUILD_DIR)
