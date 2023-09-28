# Compiler and flags
CC = mpicc
CFLAGS =

# Directories
SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# Output binary
OUT = $(BINDIR)/group_5_assignment_2

# Main build target
all: $(OUT)

# Create build and bin directories
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Linking
$(OUT): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

# Compiling and generating dependencies
-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
