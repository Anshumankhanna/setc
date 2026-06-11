#define makefile                                                               \
	"# ------------\n"                                                         \
	"# EDITABLE FLAGS\n"                                                       \
	"# ------------\n"                                                         \
	"# These are kept separate so that if required, they can be changed "      \
	"directly in the command line.\n"                                          \
	"OPTIMIZATION ::= 2\n"                                                     \
	"PROCESS_FILES ::= -pipe\n"                                                \
	"STD ::= c23\n"                                                            \
	"\n"                                                                       \
	"# ------------\n"                                                         \
	"# MULTILINE NON-EDITABLE FLAGS\n"                                         \
	"# ------------\n"                                                         \
	"DIALECT ::= -std=$(STD)\\\n"                                              \
	"	\t-fhosted\\\n"                                                          \
	"	\t-fno-asm\\\n"                                                          \
	"	\t-fpermitted-flt-eval-methods=c11\\\n"                                  \
	"	\t-fstrict-aliasing\\\n"                                                 \
	"	\t-fstrict-flex-arrays\n"                                                \
	"\n"                                                                       \
	"ERRORS ::= -pedantic-errors\\\n"                                          \
	"	\t-Wall\\\n"                                                             \
	"	\t-Walloc-zero\\\n"                                                      \
	"	\t-Warith-conversion\\\n"                                                \
	"	\t-Warray-bounds=2\\\n"                                                  \
	"	\t-Wbad-function-cast\\\n"                                               \
	"	\t-Wbidi-chars=any,ucn\\\n"                                              \
	"	\t-Wcalloc-transposed-args\\\n"                                          \
	"	\t-Wcast-align\\\n"                                                      \
	"	\t-Wcast-qual\\\n"                                                       \
	"	\t-Wconversion\\\n"                                                      \
	"	\t-Wdisabled-optimization\\\n"                                           \
	"	\t-Wdouble-promotion\\\n"                                                \
	"	\t-Wduplicated-branches\\\n"                                             \
	"	\t-Wduplicated-cond\\\n"                                                 \
	"	\t-Werror\\\n"                                                           \
	"	\t-Wextra\\\n"                                                           \
	"	\t-Wflex-array-member-not-at-end\\\n"                                    \
	"	\t-Wfloat-equal\\\n"                                                     \
	"	\t-Wformat=2\\\n"                                                        \
	"	\t-Wformat-signedness\\\n"                                               \
	"	\t-Wformat-overflow=2\\\n"                                               \
	"	\t-Wformat-truncation=2\\\n"                                             \
	"	\t-Whardened\\\n"                                                        \
	"	\t-Wimplicit-fallthrough=5\\\n"                                          \
	"	\t-Winit-self\\\n"                                                       \
	"	\t-Winline\\\n"                                                          \
	"	\t-Winvalid-pch\\\n"                                                     \
	"	\t-Winvalid-utf8\\\n"                                                    \
	"	\t-Wkeyword-macro\\\n"                                                   \
	"	\t-Wlogical-op\\\n"                                                      \
	"	\t-Wmissing-include-dirs\\\n"                                            \
	"	\t-Wmissing-noreturn\\\n"                                                \
	"	\t-Wmissing-prototypes\\\n"                                              \
	"	\t-Wmissing-variable-declarations\\\n"                                   \
	"	\t-Wmultichar\\\n"                                                       \
	"	\t-Wnested-externs\\\n"                                                  \
	"	\t-Wnull-dereference\\\n"                                                \
	"	\t-Wnormalized=nfkc\\\n"                                                 \
	"	\t-Wpadded\\\n"                                                          \
	"	\t-Wpedantic\\\n"                                                        \
	"	\t-Wpointer-arith\\\n"                                                   \
	"	\t-Wredundant-decls\\\n"                                                 \
	"	\t-Wshadow\\\n"                                                          \
	"	\t-Wstack-protector\\\n"                                                 \
	"	\t-Wstrict-overflow=5\\\n"                                               \
	"	\t-Wstringop-overflow=4\\\n"                                             \
	"	\t-Wstringop-truncation\\\n"                                             \
	"	\t-Wsuggest-attribute=noreturn\\\n"                                      \
	"	\t-Wswitch-default\\\n"                                                  \
	"	\t-Wswitch-enum\\\n"                                                     \
	"	\t-Wtrailing-whitespace=any\\\n"                                         \
	"	\t-Wtrampolines\\\n"                                                     \
	"	\t-Wtrivial-auto-var-init\\\n"                                           \
	"	\t-Wtype-limits\\\n"                                                     \
	"	\t-Wundef\\\n"                                                           \
	"	\t-Wunknown-pragmas\\\n"                                                 \
	"	\t-Wunsafe-loop-optimizations\\\n"                                       \
	"	\t-Wunused-const-variable\\\n"                                           \
	"	\t-Wunused-macros\\\n"                                                   \
	"	\t-Wuse-after-free=3\\\n"                                                \
	"	\t-Wuseless-cast\\\n"                                                    \
	"	\t-Wvector-operation-performance\\\n"                                    \
	"	\t-Wwrite-strings\\\n"                                                   \
	"	\t-Wzero-as-null-pointer-constant\n"                                     \
	"\n"                                                                       \
	"# INSTRUMENTATION ::= -fhardened\n"                                       \
	"\n"                                                                       \
	"# ------------\n"                                                         \
	"# COMPILATION FLAGS\n"                                                    \
	"# ------------\n"                                                         \
	"CC ::= gcc\n"                                                             \
	"CFLAGS ::= $(PROCESS_FILES)\\\n"                                          \
	"	\t$(DIALECT)\\\n"                                                        \
	"	\t-O$(OPTIMIZATION)\\\n"                                                 \
	"	\t$(ERRORS)\\\n"                                                         \
	"	\t$(INSTRUMENTATION)\\\n"                                                \
	"	\t-Iinclude\\\n"                                                         \
	"	\t-MMD\\\n"                                                              \
	"	\t-MP\\\n"                                                               \
	"	\t$(EXTRA_FLAGS)\n"                                                      \
	"\n"                                                                       \
	"# ------------\n"                                                         \
	"# COMPILATION ARGUMENTS\n"                                                \
	"# ------------\n"                                                         \
	"SRC_DIR ::= src\n"                                                        \
	"SRCS ::= $(wildcard $(SRC_DIR)/*.c)\n"                                    \
	"\n"                                                                       \
	"BUILD_DIR::= build\n"                                                     \
	"OBJS ::= $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)\n"                       \
	"DEPS ::= $(OBJS:.o=.d)\n"                                                 \
	"\n"                                                                       \
	"BIN_DIR ::= bin\n"                                                        \
	"TARGET ::= bin/setc.exe\n"                                                \
	"\n"                                                                       \
	"ASM_DIR ::= asms\n"                                                       \
	"ASMS ::= $(SRCS:$(SRC_DIR)/%.c=$(ASM_DIR)/%.S)\n"                         \
	"\n"                                                                       \
	"# ------------\n"                                                         \
	"# COMPILATION TARGETS\n"                                                  \
	"# ------------\n"                                                         \
	".DELETE_ON_ERROR :\n"                                                     \
	".ONESHELL :\n"                                                            \
	".PHONY : all asm clangd clean cleanasm mostlyclean run\n"                 \
	".POSIX :\n"                                                               \
	"# .SUFFIXES :\n"                                                          \
	"\n"                                                                       \
	"vpath %.c src\n"                                                          \
	"vpath %.h include\n"                                                      \
	"\n"                                                                       \
	"# Default\n"                                                              \
	"all : $(TARGET)\n"                                                        \
	"\n"                                                                       \
	"$(BUILD_DIR)/%.o : %.c | $(BUILD_DIR)\n"                                  \
	"	\t$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<\n"                             \
	"\n"                                                                       \
	"$(TARGET) : $(OBJS) | $(BIN_DIR)\n"                                       \
	"	\t$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBES) $(LDLIBS)\n"                    \
	"\n"                                                                       \
	"# Running the target\n"                                                   \
	"run : $(TARGET)\n"                                                        \
	"	\t$< $(ARGS)\n"                                                          \
	"\n"                                                                       \
	"# If we want to check the assembly output of a file.\n"                   \
	"asm : $(ASMS)\n"                                                          \
	"\n"                                                                       \
	"$(ASM_DIR)/%.S : %.c %.h | $(ASM_DIR)\n"                                  \
	"	\t$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<\n"                             \
	"\n"                                                                       \
	"# Create directories\n"                                                   \
	"$(ASM_DIR) $(BIN_DIR) $(BUILD_DIR) :\n"                                   \
	"	\t-mkdir $@\n"                                                           \
	"\n"                                                                       \
	"# Cleanup\n"                                                              \
	"RM_FLAGS ::= -I\n"                                                        \
	"CLEAN ::= -rm -r $(RM_FLAGS)\n"                                           \
	"\n"                                                                       \
	"clean :\n"                                                                \
	"	\t$(CLEAN) $(BIN_DIR) $(BUILD_DIR)\n"                                    \
	"\n"                                                                       \
	"mostlyclean :\n"                                                          \
	"	\t$(CLEAN) $(BIN_DIR)\n"                                                 \
	"\n"                                                                       \
	"# Clean asm outputs separately, not part of general compilation.\n"       \
	"cleanasm :\n"                                                             \
	"	\t$(CLEAN) $(ASM_DIR)\n"                                                 \
	"\n"                                                                       \
	"-include $(DEPS)\n"\
