#ifndef _UTIL_H_
#define _UTIL_H_ (1)

inline bool is_little_endian() {
	int n = 1;
	// little endian if true
	return ( *(char *)&n == 1) ? true : false;
}

#define switch_endian32(v) \
        (unsigned int)( ((((unsigned int)v)&0x000000FFU) << 24) | \
                        ((((unsigned int)v)&0x0000FF00U) << 8)  | \
                        ((((unsigned int)v)&0x00FF0000U) >> 8)  | \
                        ((((unsigned int)v)&0xFF000000U) >> 24) )

#define switch_endian64(v) \
        (unsigned long long)( ((((unsigned long long)v)&0x00000000000000FFULL) << 56) | \
                              ((((unsigned long long)v)&0x000000000000FF00ULL) << 40) | \
                              ((((unsigned long long)v)&0x0000000000FF0000ULL) << 24) | \
                              ((((unsigned long long)v)&0x00000000FF000000ULL) << 8)  | \
                              ((((unsigned long long)v)&0x000000FF00000000ULL) >> 8)  | \
                              ((((unsigned long long)v)&0x0000FF0000000000ULL) >> 24) | \
                              ((((unsigned long long)v)&0x00FF000000000000ULL) >> 40) | \
                              ((((unsigned long long)v)&0xFF00000000000000ULL) >> 56) )

// convert to big endian if the host is using little endian
inline uint32_t to_big_endian(uint32_t value) {
	if (is_little_endian())
		return switch_endian32(value);
	return value;
}

// convert to big endian if the host is using little endian
inline uint64_t to_big_endian(uint64_t value) {
	if (is_little_endian())
		return switch_endian64(value);
	return value;
}

// convert to little endian if the host is little endian.
inline uint32_t to_host_endian(uint32_t big_endian_value) {
	if (is_little_endian())
		return switch_endian32(big_endian_value);
	return big_endian_value;
}

// convert to little endian if the host is little endian.
inline uint64_t to_host_endian(uint64_t big_endian_value) {
	if (is_little_endian())
		return switch_endian64(big_endian_value);
	return big_endian_value;
}

inline uint64_t hash(const char *str)
{
	uint64_t hash = 5381;
    int c;

    while ( (c = (int)*str++) )
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


inline std::string concat_int(std::string str, int index) {
	std::stringstream stream;
	stream << str << index;
	return stream.str();
}


#endif // _UTIL_H_
