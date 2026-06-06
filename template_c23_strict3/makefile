# ------------
# EDITABLE FLAGS
# ------------
# These are kept separate so that if required, they can be changed directly in the command line.
OPTIMIZATION ::= -O2
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

WARNINGS ::= -Wall\
	-Wbidi-chars=any,ucn\
	-Wcast-qual\
	-Wdisabled-optimization\
	-Wextra\
	-Whardened\
	-Winvalid-pch\
	-Wlogical-op\
	-Wpadded\
	-Wshadow\
	-Wstrict-overflow=5\
	-Wtrailing-whitespace=any\
	-Wunknown-pragmas\
	-Wunsafe-loop-optimizations\
	-Wvector-operation-performance

ERROR_WARNINGS ::= \
	-Werror=alloc-zero\
	-Werror=arith-conversion\
	-Werror=array-bounds=2\
	-Werror=bad-function-cast\
	-Werror=bidi-chars\
	-Werror=calloc-transposed-args\
	-Werror=cast-align\
	-Werror=conversion\
	-Werror=double-promotion\
	-Werror=duplicated-branches\
	-Werror=duplicated-cond\
	-Werror=flex-array-member-not-at-end\
	-Werror=float-equal\
	-Werror=format=2\
	-Werror=format-signedness\
	-Werror=format-overflow=2\
	-Werror=format-truncation=2\
	-Werror=init-self\
	-Werror=inline\
	-Werror=invalid-utf8\
	-Werror=implicit-fallthrough=5\
	-Werror=jump-misses-init\
	-Werror=keyword-macro\
	-Werror=nested-externs\
	-Werror=null-dereference\
	-Werror=normalized=nfkc\
	-Werror=missing-include-dirs\
	-Werror=missing-noreturn\
	-Werror=missing-prototypes\
	-Werror=missing-variable-declarations\
	-Werror=multichar\
	-Werror=pedantic\
	-Werror=pointer-arith\
	-Werror=redundant-decls\
	-Werror=stack-protector\
	-Werror=stringop-overflow=4\
	-Werror=stringop-truncation\
	-Werror=suggest-attribute=noreturn\
	-Werror=switch-default\
	-Werror=switch-enum\
	-Werror=trampolines\
	-Werror=trivial-auto-var-init\
	-Werror=type-limits\
	-Werror=undef\
	-Werror=unused-const-variable\
	-Werror=unused-macros\
	-Werror=use-after-free=3\
	-Werror=useless-cast\
	-Werror=write-strings\
	-Werror=zero-as-null-pointer-constant

ERRORS ::= $(ERROR_WARNINGS) -pedantic-errors

# INSTRUMENTATION ::= -fhardened

# ------------
# COMPILATION FLAGS
# ------------
CC ::= gcc
CFLAGS ::= $(PROCESS_FILES)\
	$(DIALECT)\
	$(OPTIMIZATION)\
	$(WARNINGS)\
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
.PHONY : all asm clangd clean cleanasm mostlyclean run
.POSIX :
# .SUFFIXES :

vpath %.c src
vpath %.h include

# Default
all : $(TARGET)

$(BUILD_DIR)/%.o : %.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(TARGET) : $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)

# Running the target
run : $(TARGET)
	$<

# If we want to check the assembly output of a file.
asm : $(ASMS)

$(ASM_DIR)/%.S : %.c %.h | $(ASM_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<

# Create directories
$(ASM_DIR) $(BIN_DIR) $(BUILD_DIR) :
	-mkdir $@

# Cleanup
RM_FLAGS ::= -I
CLEAN ::= -rm -r $(RM_FLAGS)

clean :
	$(CLEAN) $(BIN_DIR) $(BUILD_DIR)

mostlyclean :
	$(CLEAN) $(BIN_DIR)

# Clean asm outputs separately, not part of general compilation.
cleanasm :
	$(CLEAN) $(ASM_DIR)

-include $(DEPS)
