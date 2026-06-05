# OS Course Assignment — Top-level Makefile
# Compiles all four base modules: scheduler, memory, sync, filesystem

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -O2 -I./common
LDFLAGS := -lpthread -lm

MODULES := scheduler memory sync filesystem

.PHONY: all clean test $(MODULES)

all: $(MODULES)

$(MODULES):
	$(MAKE) -C $@

test: $(MODULES)
	@echo "========================================"
	@echo " Running scheduler tests..."
	@echo "========================================"
	@./scheduler/scheduler test || true
	@echo ""
	@echo "========================================"
	@echo " Running memory tests..."
	@echo "========================================"
	@./memory/memory test || true
	@echo ""
	@echo "========================================"
	@echo " Running sync tests..."
	@echo "========================================"
	@./sync/sync test || true
	@echo ""
	@echo "========================================"
	@echo " Running filesystem tests..."
	@echo "========================================"
	@./filesystem/filesystem test || true

clean:
	@for dir in $(MODULES); do $(MAKE) -C $$dir clean; done
	@echo "All build artifacts cleaned."
