#ifndef POLY_MACROS_H
#define POLY_MACROS_H

///////////////////////////////////////////////////////////

#define __CONCAT(a, b) a##b
#define CONCAT(a, b) __CONCAT(a, b)

#define AUG(...) _, __VA_ARGS__
#define EXPAND(x) x
#define STR(x) #x

#ifdef WIN32
#define FILE_SEPERATOR '\\'
#else
#define FILE_SEPERATOR '/'
#endif

///////////////////////////////////////////////////////////

#endif