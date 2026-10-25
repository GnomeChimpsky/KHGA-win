# Cross-compile toolchain for Windows (x86_64) with MinGW-w64 (posix thread model).
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(_prefix x86_64-w64-mingw32)
find_program(CMAKE_C_COMPILER NAMES ${_prefix}-gcc-posix ${_prefix}-gcc REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES ${_prefix}-g++-posix ${_prefix}-g++ REQUIRED)
find_program(CMAKE_RC_COMPILER NAMES ${_prefix}-windres windres REQUIRED)

set(CMAKE_FIND_ROOT_PATH /usr/${_prefix})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
