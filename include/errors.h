#ifndef ERRORS_H
#define ERRORS_H

typedef enum status { OK, ERR } status;

#define RESULT(T, E)                                                           \
	struct {                                                                   \
		status type : sizeof(u(8));                                            \
		union {                                                                \
			E error;                                                           \
			T val;                                                             \
		} output;                                                              \
	} RESULT_##T##_##E

#endif
