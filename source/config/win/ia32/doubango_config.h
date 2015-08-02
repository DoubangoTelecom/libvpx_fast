#ifndef DOUBANGO_CONFIG_H
#define DOUBANGO_CONFIG_H

#define DOUBANGO_IS_ALIGNED(p, a) (!((uintptr_t)(p) & ((a) - 1)))
#define DOUBANGO_ASM_INLINE		1
#define DOUBANGO_INTRINSIC		1

#endif /* DOUBANGO_CONFIG_H */