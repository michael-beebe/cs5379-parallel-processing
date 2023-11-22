# Compilers
CC = gcc
NVCC = nvcc

# Flags
CFLAGS =
NVCCFLAGS = -arch=sm_70 # Adjust this for your GPU architecture, this is for VOLTA architecture

# Directories
SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Files
CSRCS = $(wildcard $(SRCDIR)/*.c)
CU_SRCS = $(wildcard $(SRCDIR)/*.cu)
COBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CSRCS))
CU_OBJS = $(patsubst $(SRCDIR)/%.cu,$(BUILDDIR)/%.o,$(CU_SRCS))
DEPS = $(COBJS:.o=.d) $(CU_OBJS:.o=.d)

# Output binary
OUT = $(BINDIR)/group_5_assignment_6

# Main build target
all: $(OUT)

# Create build and bin directories
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Linking
$(OUT): $(COBJS) $(CU_OBJS) | $(BINDIR)
	$(NVCC) $(NVCCFLAGS) -o $(OUT) $(COBJS) $(CU_OBJS)

# Compiling C sources
-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Compiling CUDA sources
$(BUILDDIR)/%.o: $(SRCDIR)/%.cu | $(BUILDDIR)
	$(NVCC) $(NVCCFLAGS) -MMD -c $< -o $@

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
