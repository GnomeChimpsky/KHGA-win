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
# KHG_GALAXYREVERB: effects mixed on a loopback device with the retail Galaxy zone echo (GalaxyReverb.cpp).
target_sources(NOpenALDrv PRIVATE "${UE1_ROOT}/Source/NOpenALDrv/GalaxyReverb.cpp")
target_compile_definitions(NOpenALDrv PRIVATE KHG_GALAXYREVERB)
target_link_libraries(NOpenALDrv ff_avformat ff_avcodec ff_avutil ff_swresample)
target_include_directories(NOpenALDrv PRIVATE "${FFMPEG_INCLUDE_DIR}")
# M8: frame pacing and the -khgtiming test mode.
target_sources(Unreal PRIVATE "${UE1_ROOT}/Source/Unreal/Src/KHGTiming.cpp")
# Movies and CD music copied from the game CD, and the game installed from disc 1 (KHGDiscImport.cpp).
target_sources(Unreal PRIVATE "${UE1_ROOT}/Source/Unreal/Src/KHGDiscImport.cpp")
# unshield + zlib (inflate only) unpack the retail installer's DATA1.CAB (InstallShield 5).
set(KHG_THIRDPARTY "${CMAKE_CURRENT_LIST_DIR}/../app/src/main/cpp/thirdparty")
add_library(khg_zlib STATIC
    "${KHG_THIRDPARTY}/zlib/adler32.c" "${KHG_THIRDPARTY}/zlib/crc32.c" "${KHG_THIRDPARTY}/zlib/inffast.c"
    "${KHG_THIRDPARTY}/zlib/inflate.c" "${KHG_THIRDPARTY}/zlib/inftrees.c" "${KHG_THIRDPARTY}/zlib/zutil.c")
target_include_directories(khg_zlib PUBLIC "${KHG_THIRDPARTY}/zlib")
add_library(khg_unshield STATIC
    "${KHG_THIRDPARTY}/unshield/lib/component.c" "${KHG_THIRDPARTY}/unshield/lib/converter.c"
    "${KHG_THIRDPARTY}/unshield/lib/directory.c" "${KHG_THIRDPARTY}/unshield/lib/file.c"
    "${KHG_THIRDPARTY}/unshield/lib/file_group.c" "${KHG_THIRDPARTY}/unshield/lib/helper.c"
    "${KHG_THIRDPARTY}/unshield/lib/libunshield.c" "${KHG_THIRDPARTY}/unshield/lib/log.c"
    "${KHG_THIRDPARTY}/unshield/lib/md5/md5c.c")
target_include_directories(khg_unshield PUBLIC "${KHG_THIRDPARTY}/unshield/lib" PRIVATE "${KHG_THIRDPARTY}/unshield")
target_compile_options(khg_unshield PRIVATE -w)
target_link_libraries(khg_unshield PUBLIC khg_zlib)
target_link_libraries(Unreal khg_unshield)
# The launcher defines its own WinMain (SDLLaunch.cpp), so SDL2main is not linked.
target_link_libraries(Unreal xmp_static shell32 ole32 uuid comdlg32)   # shell32/ole32/uuid: Saved Games folder, game folder dialog (KHGDesktopConfig.cpp)

# M9: build version, from the git tag (vX.Y.Z) or -DKHG_VERSION=...; shown in the log, crash reports and
# the exe's version info.
if(NOT KHG_VERSION)
    execute_process(COMMAND git describe --tags --always --dirty --abbrev=7
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" OUTPUT_VARIABLE KHG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(NOT KHG_VERSION)
        set(KHG_VERSION "unknown")
    endif()
endif()
set(KHG_VERSION_NUM "0,0,0,0")
if(KHG_VERSION MATCHES "^v?([0-9]+)\\.([0-9]+)\\.([0-9]+)")
    set(KHG_VERSION_NUM "${CMAKE_MATCH_1},${CMAKE_MATCH_2},${CMAKE_MATCH_3},0")
endif()
message(STATUS "KHGWin version: ${KHG_VERSION}")
# A generated header, not -D flags: windres runs its preprocessor through a shell, which loses the quotes.
file(WRITE "${CMAKE_BINARY_DIR}/khgversion/KHGVersion.h.tmp"
    "#define KHG_VERSION \"${KHG_VERSION}\"\n#define KHG_VERSION_NUM ${KHG_VERSION_NUM}\n")
configure_file("${CMAKE_BINARY_DIR}/khgversion/KHGVersion.h.tmp" "${CMAKE_BINARY_DIR}/khgversion/KHGVersion.h" COPYONLY)
target_include_directories(Unreal PRIVATE "${CMAKE_BINARY_DIR}/khgversion")
target_include_directories(Unreal PRIVATE "${UE1_ROOT}/Source/Unreal/Src/Res")   # Unreal.ico for KHGWin.rc

# The launcher is installed as KHGWin.exe so it never overwrites the retail Khg.exe.
set_target_properties(Unreal PROPERTIES OUTPUT_NAME "KHGWin")

# --- Staging: the release folder (KHGWin.exe and its files; runs from anywhere, see KHGDesktopChooseGameDir) ---
set(KHG_STAGE_DIR "${CMAKE_BINARY_DIR}/stage/KHGWin")
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
