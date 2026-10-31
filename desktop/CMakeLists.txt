cmake_minimum_required(VERSION 3.22)
project(KHGDesktop LANGUAGES C CXX)

# Desktop (Windows) build of the shared engine tree under app/src/main/cpp.
# The Android build keeps using app/src/main/cpp/CMakeLists.txt through Gradle;
# this file is the desktop counterpart and links everything into one executable.
#
# Configure with one of the toolchains in desktop/cmake/, e.g.
#   cmake -S desktop -B build/win64 -DCMAKE_TOOLCHAIN_FILE=desktop/cmake/mingw-w64-x86_64.cmake \
#         -DFFMPEG_ROOT=<ffmpeg install prefix>
# See docs/BUILD-WINDOWS.md.

if(NOT WIN32)
    message(FATAL_ERROR "The desktop build currently targets Windows only.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../app/src/main/cpp")
get_filename_component(NATIVE_ROOT "${NATIVE_ROOT}" ABSOLUTE)
set(UE1_ROOT "${NATIVE_ROOT}/UE1")
set(SDL2_ROOT "${NATIVE_ROOT}/thirdparty/SDL2")
set(OPENAL_ROOT "${NATIVE_ROOT}/thirdparty/openal-soft")
set(LIBXMP_ROOT "${NATIVE_ROOT}/thirdparty/libxmp")

set(FFMPEG_ROOT "" CACHE PATH "Install prefix of the MinGW FFmpeg build (desktop/scripts/build-ffmpeg.sh)")
if(NOT EXISTS "${FFMPEG_ROOT}/include/libavcodec/avcodec.h")
    message(FATAL_ERROR "FFMPEG_ROOT does not point at an FFmpeg install. Build one with desktop/scripts/build-ffmpeg.sh.")
endif()

# Keep build-machine paths out of __FILE__ (assert/crash lines in the log).
add_compile_options(
    "-ffile-prefix-map=${NATIVE_ROOT}/="
    "-ffile-prefix-map=${CMAKE_BINARY_DIR}/=")

# No dependency on the MinGW runtime DLLs next to the exe.
add_link_options(-static-libgcc -static-libstdc++ -static)

# --- libxmp (UMX module music), static ---
set(BUILD_STATIC ON CACHE BOOL "" FORCE)
set(BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(BUILD_LITE OFF CACHE BOOL "" FORCE)
set(WITH_UNIT_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIBXMP_ROOT}" "${CMAKE_BINARY_DIR}/libxmp" EXCLUDE_FROM_ALL)
if(TARGET xmp_static)
    target_compile_options(xmp_static PRIVATE -Wno-unused-function)
endif()

# --- SDL2, static (zlib license) ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL2_DISABLE_SDL2MAIN ON CACHE BOOL "" FORCE)
add_subdirectory("${SDL2_ROOT}" "${CMAKE_BINARY_DIR}/SDL2" EXCLUDE_FROM_ALL)

# --- OpenAL Soft, shared (LGPL: ships as a replaceable DLL) ---
set(LIBTYPE SHARED CACHE STRING "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_WASAPI ON CACHE BOOL "" FORCE)
add_subdirectory("${OPENAL_ROOT}" "${CMAKE_BINARY_DIR}/openal-soft" EXCLUDE_FROM_ALL)
foreach(_tgt IN ITEMS OpenAL alsoft.common)
    if(TARGET ${_tgt})
        target_compile_options(${_tgt} PRIVATE -Wno-sign-conversion)
    endif()
endforeach()

# --- FFmpeg (LGPL), shared DLLs from desktop/scripts/build-ffmpeg.sh ---
foreach(_ff avutil avcodec avformat swscale swresample)
    add_library(ff_${_ff} SHARED IMPORTED GLOBAL)
    file(GLOB _ff_dll "${FFMPEG_ROOT}/bin/${_ff}-*.dll")
    set_target_properties(ff_${_ff} PROPERTIES
        IMPORTED_LOCATION "${_ff_dll}"
        IMPORTED_IMPLIB "${FFMPEG_ROOT}/lib/lib${_ff}.dll.a"
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_ROOT}/include")
endforeach()
set(FFMPEG_INCLUDE_DIR "${FFMPEG_ROOT}/include" CACHE PATH "" FORCE)

# --- Options consumed by the UE1 CMake files ---
set(USE_SDL ON CACHE BOOL "" FORCE)
set(BUILD_EDITOR OFF CACHE BOOL "" FORCE)
set(BUILD_SOFTDRV OFF CACHE BOOL "" FORCE)
set(BUILD_NOPENGLDRV OFF CACHE BOOL "" FORCE)
set(BUILD_NOPENGLESDRV OFF CACHE BOOL "" FORCE)
set(BUILD_VULKANDRV ON CACHE BOOL "" FORCE)
set(BUILD_NULLSOUNDDRV ON CACHE BOOL "" FORCE)
set(BUILD_NOPENALDRV ON CACHE BOOL "" FORCE)
set(BUILD_WINDRV OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC ON CACHE BOOL "" FORCE)
set(UE1_NATIVE_64BIT ON CACHE BOOL "" FORCE)
set(UE1_LAUNCHER_RC "${CMAKE_CURRENT_LIST_DIR}/res/KHGWin.rc")

set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include" CACHE PATH "" FORCE)
set(SDL2_LIBRARY SDL2::SDL2-static CACHE STRING "" FORCE)
set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT}/include" CACHE PATH "" FORCE)
set(OPENAL_LIBRARY OpenAL CACHE STRING "" FORCE)
set(LIBXMP_INCLUDE_DIR "${LIBXMP_ROOT}/include" CACHE PATH "" FORCE)
set(LIBXMP_LIBRARY xmp_static CACHE STRING "" FORCE)

# KHG_FMV: enables the engine-side FMV hooks (playavi present in UGameEngine::Draw) that Android gets via PLATFORM_ANDROID.
add_compile_definitions(WITH_SDL2 WITH_OPENAL KHG_FMV)

list(APPEND CMAKE_MODULE_PATH "${UE1_ROOT}/Source/cmake")
add_subdirectory("${UE1_ROOT}/Source" "${CMAKE_BINARY_DIR}/UE1")

target_link_libraries(NSDLDrv ff_avformat ff_avcodec ff_avutil ff_swscale ff_swresample)
target_include_directories(NSDLDrv PRIVATE "${FFMPEG_INCLUDE_DIR}")
# KHG_CDMUSIC: PlayerPawn.CdTrack is played from ripped track files (NOpenALDrv/CDMusic.cpp).
target_sources(NOpenALDrv PRIVATE "${UE1_ROOT}/Source/NOpenALDrv/CDMusic.cpp")
target_compile_definitions(NOpenALDrv PRIVATE KHG_CDMUSIC)
target_link_libraries(NOpenALDrv ff_avformat ff_avcodec ff_avutil ff_swresample)
target_include_directories(NOpenALDrv PRIVATE "${FFMPEG_INCLUDE_DIR}")
# The launcher defines its own WinMain (SDLLaunch.cpp), so SDL2main is not linked.
target_link_libraries(Unreal xmp_static)
target_include_directories(Unreal PRIVATE "${UE1_ROOT}/Source/Unreal/Src/Res")   # Unreal.ico for KHGWin.rc

# The launcher is installed as KHGWin.exe so it never overwrites the retail Khg.exe.
set_target_properties(Unreal PROPERTIES OUTPUT_NAME "KHGWin")

# --- Staging: everything that goes into the retail System folder ---
set(KHG_STAGE_DIR "${CMAKE_BINARY_DIR}/stage/System")
add_custom_target(stage ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${KHG_STAGE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Unreal>" "${KHG_STAGE_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:OpenAL>" "${KHG_STAGE_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:ff_avutil>" "$<TARGET_FILE:ff_avcodec>" "$<TARGET_FILE:ff_avformat>"
        "$<TARGET_FILE:ff_swscale>" "$<TARGET_FILE:ff_swresample>" "${KHG_STAGE_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/config/KHGWinDefaults.ini" "${CMAKE_CURRENT_LIST_DIR}/config/KHGWin.int"
        "${KHG_STAGE_DIR}/"
    DEPENDS Unreal OpenAL
    VERBATIM)
