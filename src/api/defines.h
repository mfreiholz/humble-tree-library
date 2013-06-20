#ifndef _HTL_DEFINES_HEADER_
#define _HTL_DEFINES_HEADER_

// Makes it possible to use the library as shared and static lib.
#if defined(HTL_EXPORT)
  #define HTL_EXPORT_API __declspec(dllexport)
#else
  #define HTL_EXPORT_API __declspec(dllimport)
#endif

// Namespace define.
// Always use these defines to keep the correct name.
#define NAMESPACE_BEGIN namespace htl {
#define NAMESPACE_END }

#endif
