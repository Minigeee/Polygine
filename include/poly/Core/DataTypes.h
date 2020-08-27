#ifndef POLY_DATA_TYPES_H
#define POLY_DATA_TYPES_H

#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>

#include <stdint.h>

namespace poly
{

// Integer types
typedef int8_t		Int8;
typedef uint8_t		Uint8;

typedef int16_t		Int16;
typedef uint16_t	Uint16;

typedef int32_t		Int32;
typedef uint32_t	Uint32;

typedef int64_t		Int64;
typedef uint64_t	Uint64;

// Data structure implementations
template<class Key, class T>
using HashMap = tsl::hopscotch_map<Key, T>;

template<class Key>
using HashSet = tsl::hopscotch_set<Key>;

}

#endif