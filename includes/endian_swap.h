#ifndef ENDIAN_SWAP_H
# define ENDIAN_SWAP_H

# define SWAP_U16(x) \
    (x>>8) | (x<<8)
    
# define SWAP_U32(x)      \
    ((x>>24) & 0xff) |    \
    ((x>>8) & 0xff00) |   \
    ((x<<8) & 0xff0000) | \
    ((x<<24) & 0xff000000)
    
# define SWAP_U64(x)                        \
    (((x) >> 56) & 0x00000000000000ffULL) | \
    (((x) >> 40) & 0x000000000000ff00ULL) | \
    (((x) >> 24) & 0x0000000000ff0000ULL) | \
    (((x) >> 8)  & 0x00000000ff000000ULL) | \
    (((x) << 8)  & 0x000000ff00000000ULL) | \
    (((x) << 24) & 0x0000ff0000000000ULL) | \
    (((x) << 40) & 0x00ff000000000000ULL) | \
    (((x) << 56) & 0xff00000000000000ULL)

#endif
