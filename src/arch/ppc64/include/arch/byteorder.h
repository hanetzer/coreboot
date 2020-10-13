/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _BYTEORDER_H
#define _BYTEORDER_H

#if CONFIG(PPC64_LE)
#define __LITTLE_ENDIAN 1234
#else
#define __BIG_ENDIAN 4321
#endif

#endif /* _BYTEORDER_H */
