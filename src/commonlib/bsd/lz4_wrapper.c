/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0-only */

#include <commonlib/bsd/compression.h>
#include <commonlib/bsd/helpers.h>
#include <commonlib/bsd/sysincludes.h>
#include <stdint.h>
#include <string.h>

/* LZ4 comes with its own supposedly portable memory access functions, but they
 * seem to be very inefficient in practice (at least on ARM64). Since coreboot
 * knows about endinaness and allows some basic assumptions (such as unaligned
 * access support), we can easily write the ones we need ourselves. */
static uint16_t LZ4_readLE16(const void *src)
{
	return le16toh(*(const uint16_t *)src);
}
static void LZ4_copy8(void *dst, const void *src)
{
/* ARM32 needs to be a special snowflake to prevent GCC from coalescing the
 * access into LDRD/STRD (which don't support unaligned accesses). */
#ifdef __arm__	/* ARMv < 6 doesn't support unaligned accesses at all. */
	#if defined(__COREBOOT_ARM_ARCH__) && __COREBOOT_ARM_ARCH__ < 6
		int i;
		for (i = 0; i < 8; i++)
			((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
	#else
		uint32_t x0, x1;
		__asm__ ("ldr %[x0], [%[src]]"
			: [x0]"=r"(x0)
			: [src]"r"(src), "m"(*(const uint32_t *)src));
		__asm__ ("ldr %[x1], [%[src], #4]"
			: [x1]"=r"(x1)
			: [src]"r"(src), "m"(*(const uint32_t *)(src + 4)));
		__asm__ ("str %[x0], [%[dst]]"
			: "=m"(*(uint32_t *)dst)
			: [x0]"r"(x0), [dst]"r"(dst));
		__asm__ ("str %[x1], [%[dst], #4]"
			: "=m"(*(uint32_t *)(dst + 4))
			: [x1]"r"(x1), [dst]"r"(dst));
	#endif
#elif defined(__riscv)
	/* RISC-V implementations may trap on any unaligned access. */
	int i;
	for (i = 0; i < 8; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
#else
	*(uint64_t *)dst = *(const uint64_t *)src;
#endif
}

typedef  uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef  int32_t S32;
typedef uint64_t U64;

#define FORCE_INLINE static __always_inline
#define likely(expr) __builtin_expect((expr) != 0, 1)
#define unlikely(expr) __builtin_expect((expr) != 0, 0)

/* Unaltered (just removed unrelated code) from github.com/Cyan4973/lz4/dev. */
#include "lz4.c.inc"	/* #include for inlining, do not link! */

#define LZ4F_MAGICNUMBER 0x184D2204
#define LZ4F_BLOCKUNCOMPRESSED_FLAG 0x80000000U

size_t ulz4fn(const void *src, size_t srcn, void *dst, size_t dstn)
{
	const void *in = src;
	void *out = dst;
	size_t out_size = 0;
	int has_block_checksum;

	{ /* With in-place decompression the header may become invalid later. */
		uint32_t magic;
		uint8_t flags, version, independent_blocks, has_content_size;
		uint8_t block_desc;

		if (srcn < sizeof(uint32_t) + 3*sizeof(uint8_t))
			return 0;	/* input overrun */

		magic = le32toh(*(const uint32_t *)in);
		in += sizeof(uint32_t);
		flags = *(const uint8_t *)in;
		in += sizeof(uint8_t);
		block_desc = *(const uint8_t *)in;
		in += sizeof(uint8_t);

		version = (flags >> 6) & 0x3;
		independent_blocks = (flags >> 5) & 0x1;
		has_block_checksum = (flags >> 4) & 1;
		has_content_size = (flags >> 3) & 0x1;

		/* We assume there's always only a single, standard frame. */
		if (magic != LZ4F_MAGICNUMBER || version != 1)
			return 0;	/* unknown format */
		if ((flags & 0x03) || (block_desc & 0x8f))
			return 0;	/* reserved bits must be zero */
		if (!independent_blocks)
			return 0;	/* we don't support block dependency */

		if (has_content_size) {
			if (srcn < sizeof(uint32_t) + 3*sizeof(uint8_t) + sizeof(uint64_t))
				return 0; /* input overrun */
			in += sizeof(uint64_t);
		}
		/* Header checksum byte */
		in += sizeof(uint8_t);
	}

	while (1) {
		uint32_t block_header, block_size;

		block_header = le32toh(*(const uint32_t *)in);
		in += sizeof(uint32_t);
		block_size = block_header & ~LZ4F_BLOCKUNCOMPRESSED_FLAG;

		if ((size_t)(in - src) + block_size > srcn)
			break;          /* input overrun */

		if ((size_t)(in - src) + block_size > srcn)
			break;			/* input overrun */

		if (!block_size) {
			out_size = out - dst;
			break;			/* decompression successful */
		}

		if (block_header & LZ4F_BLOCKUNCOMPRESSED_FLAG) {
			size_t size = MIN((uintptr_t)block_size, (uintptr_t)dst
				+ dstn - (uintptr_t)out);
			memcpy(out, in, size);
			if (size < block_size)
				break;		/* output overrun */
			out += size;
		} else {
			/* constant folding essential, do not touch params! */
			int ret = LZ4_decompress_generic(in, out, block_size,
					dst + dstn - out, endOnInputSize,
					full, 0, noDict, out, NULL, 0);
			if (ret < 0)
				break;		/* decompression error */
			out += ret;
		}

		in += block_size;
		if (has_block_checksum)
			in += sizeof(uint32_t);
	}

	return out_size;
}

size_t ulz4f(const void *src, void *dst)
{
	/* LZ4 uses signed size parameters, so can't just use ((u32)-1) here. */
	return ulz4fn(src, 1*GiB, dst, 1*GiB);
}
