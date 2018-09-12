#include <cstdlib>

enum flags { none, verbose, extra_verbose };
extern int FLAGS;
extern int THREADS;
extern int RETURN_CODE;

extern double RANDOM_ANCHOR_PROP;
extern long unsigned int BOOTSTRAP;

/**
 * @brief This macro is used to print a warning and make the program exit with
 * an failure exit code, later.
 */
#define soft_err(...)                                                          \
	do {                                                                       \
		RETURN_CODE |= EXIT_FAILURE;                                           \
		warn(__VA_ARGS__);                                                     \
	} while (0)

/**
 * @brief This macro is used to print a warning and make the program exit with
 * an failure exit code, later.
 */
#define soft_errx(...)                                                         \
	do {                                                                       \
		RETURN_CODE |= EXIT_FAILURE;                                           \
		warnx(__VA_ARGS__);                                                    \
	} while (0)
