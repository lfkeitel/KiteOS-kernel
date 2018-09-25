#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	puts("kernel: panic: abort()\n");
	asm volatile("hlt");
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	puts("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}
