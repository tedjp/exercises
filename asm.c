/* This program is a demonstration of combining compile- and run-time
 * detection of CPU features and using a handwritten implementation when
 * the CPU supports it.
 *
 * The result is that this uses one of three implementations of
 * bit-counting, depending on the CPU and compiler:
 *
 * - If the CPU has a POPCNT instruction on amd64, it is always used via
 *   hand-coded assembly (runtime detection w/ fallback); otherwise
 *
 * - If compiled with -march=native on Nehalem or later (or an
 *   equivalent modern -march= that includes POPCNT), POPCNT is used via
 *   GCC's __builtin_popcount() (compile-time detection; no fallback);
 *   otherwise
 *
 * - If the compiler is GCC, a fairly efficient branchless algorithm of
 *   about 30 instructions is used (this is what you'll get if you just
 *   use __builtin_popcount() without a custom -march= option) — POPCNT
 *   is not used; otherwise
 *
 * - The fallback is a clear but relatively slow implementation that has
 *   branches. POPCNT is not used.
 *
 * The advantage in all this is that you'll get the best of both worlds
 * in the same binary: single-instruction POPCNT when the CPU supports
 * it, and seamless backwards compatibility when it doesn't.
 *
 * An option unused in this example is linker-based Function
 * Multiversioning, that replaces the need for an explicit CPUID call
 * with one done by the runtime linker;
 * <https://gcc.gnu.org/onlinedocs/gcc/Function-Multiversioning.html>,
 * but it's more fun to execute the CPUID instruction ourselves.
 *
 * © 2019 Ted Percival
 * MIT license, see LICENSE file.
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __GNUC__

static unsigned count_bits_generic(uint32_t value) {
    return __builtin_popcount(value);
}

#else /* !__GNUC__ */

// This is not the best algorithm.
// See a disassembled __builtin_popcount() for a better one.
static unsigned count_bits_generic(uint32_t value) {
    unsigned bits = 0;

    while (value) {
        if ((value & 0x1) == 1)
            ++bits;

        value >>= 1;
    }

    return bits;
}

#endif /* !__GNUC__ */

#ifdef __x86_64__

static unsigned (*count_bits)(uint32_t);

static bool has_popcnt_cpuid() {
    register uint32_t cpuid_ecx asm("ecx");

    asm (
            "movl $1, %%eax\n\t" // feature flags request
            "cpuid"
            : "=rm" (cpuid_ecx) /* outputs */
            : /* no inputs */
            : "eax", "ebx", /*"ecx",*/ "edx" /* clobbers */
        );

    return cpuid_ecx & (1 << 23);
}

static unsigned definitely_popcnt(uint32_t value) {
    unsigned bits;

    asm (
            "popcnt %1, %0"
            : "=rm" (bits)
            : "irm" (value)
        );

    return bits;
}

__attribute__((constructor))
static void determine_whether_popcnt_exists() {
    count_bits = has_popcnt_cpuid() ? &definitely_popcnt : &count_bits_generic;
}

#else /* !__x86_64__ */

static unsigned count_bits(uint32_t value) {
    return count_bits_generic(value);
}

#endif

int main(void) {
    return count_bits(0xffffffff);
}
