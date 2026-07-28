# ------------
# EDITABLE FLAGS
# ------------
# These are kept separate so that if required, they can be changed directly in the command line.
OPTIMIZATION ::= 2
PROCESS_FILES ::= -pipe
STD ::= c23

# ------------
# MULTILINE NON-EDITABLE FLAGS
# ------------
DIALECT ::= -std=$(STD)\
	-fhosted\
	-fno-asm\
	-fpermitted-flt-eval-methods=c11\
	-fstrict-aliasing\
	-fstrict-flex-arrays

ERRORS ::= -pedantic-errors\
	-Wall\
	-Walloc-zero\
	-Warith-conversion\
	-Warray-bounds=2\
	-Wbad-function-cast\
	-Wbidi-chars=any,ucn\
	-Wcalloc-transposed-args\
	-Wcast-align\
	-Wcast-qual\
	-Wconversion\
	-Wdisabled-optimization\
	-Wdouble-promotion\
	-Wduplicated-branches\
	-Wduplicated-cond\
	-Werror\
	-Wextra\
	-Wflex-array-member-not-at-end\
	-Wfloat-equal\
	-Wformat=2\
	-Wformat-signedness\
	-Wformat-overflow=2\
	-Wformat-truncation=2\
	-Whardened\
	-Wimplicit-fallthrough=5\
	-Winit-self\
	-Winline\
	-Winvalid-pch\
	-Winvalid-utf8\
	-Wkeyword-macro\
	-Wlogical-op\
	-Wmissing-include-dirs\
	-Wmissing-noreturn\
	-Wmissing-prototypes\
	-Wmissing-variable-declarations\
	-Wmultichar\
	-Wnested-externs\
	-Wnull-dereference\
	-Wnormalized=nfkc\
	-Wpadded\
	-Wpedantic\
	-Wpointer-arith\
	-Wredundant-decls\
	-Wshadow\
	-Wstack-protector\
	-Wstrict-overflow=5\
	-Wstringop-overflow=4\
	-Wstringop-truncation\
	-Wsuggest-attribute=noreturn\
	-Wswitch-default\
	-Wswitch-enum\
	-Wtrailing-whitespace=any\
	-Wtrampolines\
	-Wtrivial-auto-var-init\
	-Wtype-limits\
	-Wundef\
	-Wunknown-pragmas\
	-Wunsafe-loop-optimizations\
	-Wunused-const-variable\
	-Wunused-macros\
	-Wuse-after-free=3\
	-Wuseless-cast\
	-Wvector-operation-performance\
	-Wwrite-strings\
	-Wzero-as-null-pointer-constant

# INSTRUMENTATION ::= -fhardened

# ------------
# COMPILATION FLAGS
# ------------
CC ::= gcc
CFLAGS ::= $(PROCESS_FILES)\
	$(DIALECT)\
	-O$(OPTIMIZATION)\
	$(ERRORS)\
	$(INSTRUMENTATION)\
	-Iinclude\
	-MMD\
	-MP\
	$(EXTRA_FLAGS)

# ------------
# COMPILATION ARGUMENTS
# ------------
SRC_DIR ::= src
SRCS ::= $(wildcard $(SRC_DIR)/*.c)

BUILD_DIR::= build
OBJS ::= $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS ::= $(OBJS:.o=.d)

BIN_DIR ::= bin
TARGET ::= bin/setc.exe

ASM_DIR ::= asms
ASMS ::= $(SRCS:$(SRC_DIR)/%.c=$(ASM_DIR)/%.S)

# ------------
# COMPILATION TARGETS
# ------------
.DELETE_ON_ERROR :
.ONESHELL :
.POSIX :
# .SUFFIXES :

vpath %.c src
vpath %.h include

# Default
.PHONY : all
all : $(TARGET)

$(BUILD_DIR)/%.o : %.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(TARGET) : $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

# Running the target
.PHONY : run
run : $(TARGET)
	$< $(ARGS)

# If we want to check the assembly output of a file.
.PHONY : asm
asm : $(ASMS)

$(ASM_DIR)/%.S : %.c %.h | $(ASM_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<

# Create directories
$(ASM_DIR) $(BIN_DIR) $(BUILD_DIR) :
	-mkdir $@

.PHONY : stat
stat :
	git status -sb --ignored

.PHONY : new
new :
	./scripts/new ${ARGS}

# Cleanup
RM_FLAGS ::= -I
CLEAN ::= -rm -r $(RM_FLAGS)

.PHONY : clean
clean :
	$(CLEAN) $(BIN_DIR) $(BUILD_DIR)

.PHONY : mostlyclean
mostlyclean :
	$(CLEAN) $(BIN_DIR)

.PHONY : cleanasm
# Clean asm outputs separately, not part of general compilation.
cleanasm :
	$(CLEAN) $(ASM_DIR)

-include $(DEPS)
