/**
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2019 (C) Fabian Klötzl
 */

#include "revseqcmp.h"

#include <assert.h>

#define UNLIKELY(X) __builtin_expect((X), 0)

/** @brief Check whether two characters are complementary.
 * @param c - One nucleotide.
 * @param d - A nucleotide from the other sequence.
 * @returns true iff the two nucleotides are complements.
 */
int is_complement(char c, char d)
{
	int xorr = c ^ d;
	return (xorr & 6) == 4;
}

size_t revseqcmp_generic(const char *begin, const char *other, size_t length)
{
	assert(begin != NULL);
	assert(other != NULL);

	size_t substitutions = 0;
	size_t i = 0;

	for (; i < length; i++) {
		if (UNLIKELY(!is_complement(begin[i], other[length - 1 - i]))) {
			substitutions++;
		}
	}

	return substitutions;
}

#ifdef ENABLE_X86_SIMD

revseqcmp_fn *revseqcmp_select(void)
{
	// As ifunc resolvers are called before any constructors run, we explicitly
	// have to initialize the cpu model detector.
	// https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html
	__builtin_cpu_init();

	if (__builtin_cpu_supports("popcnt") && __builtin_cpu_supports("ssse3")) {
		return revseqcmp_ssse3;
	} else {
		return revseqcmp_generic;
	}
}

#if __has_attribute(ifunc)

size_t revseqcmp(const char *begin, const char *other, size_t length)
	__attribute__((ifunc("revseqcmp_select")));

#else

size_t revseqcmp(const char *begin, const char *other, size_t length)
{
	return (revseqcmp_select())(begin, other, length);
}

#endif
#else

size_t revseqcmp(const char *begin, const char *other, size_t length)
{
	return revseqcmp_generic(begin, other, length);
}

#endif
