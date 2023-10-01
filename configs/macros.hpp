#ifndef CONFIGS_MACROS_HPP
#define CONFIGS_MACROS_HPP

#include <cassert>

#define NO_DEFAULT_CTOR(TypeName) TypeName() = delete;
#define NO_COPY_CTOR(TypeName) TypeName(const TypeName &) = delete;
#define NO_COPY_OPERATOR(TypeName) void operator=(const TypeName &) = delete;
#define NO_MOVE_CTOR(TypeName) TypeName(TypeName &&) = delete;
#define NO_MOVE_OPERATOR(TypeName) TypeName &operator=(TypeName &&) = delete;

// Disabling copy ctor and copy assignment operator.
#define NO_COPY_SEMANTIC(TypeName) \
    NO_COPY_CTOR(TypeName)         \
    NO_COPY_OPERATOR(TypeName)

// Disabling move ctor and move assignment operator.
#define NO_MOVE_SEMANTIC(TypeName) \
    NO_MOVE_CTOR(TypeName)         \
    NO_MOVE_OPERATOR(TypeName)

#endif  // CONFIGS_MACROS_HPP