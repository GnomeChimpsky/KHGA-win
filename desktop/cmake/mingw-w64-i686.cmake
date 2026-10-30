# Cross-compile toolchain for Windows (i686) with MinGW-w64 (posix thread model).
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(_prefix i686-w64-mingw32)
find_program(CMAKE_C_COMPILER NAMES ${_prefix}-gcc-posix ${_prefix}-gcc REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES ${_prefix}-g++-posix ${_prefix}-g++ REQUIRED)
find_program(CMAKE_RC_COMPILER NAMES ${_prefix}-windres windres REQUIRED)

set(CMAKE_FIND_ROOT_PATH /usr/${_prefix})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# [KHG] SSE2 floating point, as the x64 build uses: with the default x87 maths the two builds round
# differently and long fights drift apart (M2 input replay test). Every Windows 10/11 PC has SSE2.
set(CMAKE_C_FLAGS_INIT "-msse2 -mfpmath=sse")
set(CMAKE_CXX_FLAGS_INIT "-msse2 -mfpmath=sse")
