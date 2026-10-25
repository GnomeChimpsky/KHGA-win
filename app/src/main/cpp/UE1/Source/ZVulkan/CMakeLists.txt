# [KHG] Trimmed ZVulkan build for the KHG port (Android/NDK and Windows desktop).
# Source: dpjudas/UT99VulkanDrv ZVulkan. glslang/runtime-GLSL is EXCLUDED — we precompile shaders to
# SPIR-V offline (see VulkanDrv/shaders). volk owns the Vulkan prototypes (dlopen loader); VMA routes
# through volk's loaded pointers (VMA_STATIC_VULKAN_FUNCTIONS=1 + volk.h, set inside vk_mem_alloc.cpp).
cmake_minimum_required(VERSION 3.18)
project(ZVulkan CXX C)

add_library(ZVulkan STATIC
    src/vulkanbuilders.cpp
    src/vulkandevice.cpp
    src/vulkaninstance.cpp
    src/vulkansurface.cpp
    src/vulkanswapchain.cpp
    src/vk_mem_alloc/vk_mem_alloc.cpp
    src/volk/volk.c
)

set_target_properties(ZVulkan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(ZVulkan PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zvulkan
)
target_include_directories(ZVulkan PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# VK_NO_PROTOTYPES: volk supplies function pointers (no link-time loader; it is loaded at runtime ->
# graceful failure on driverless devices).
target_compile_definitions(ZVulkan PUBLIC VK_NO_PROTOTYPES)

if(ANDROID)
    # VK_USE_PLATFORM_ANDROID_KHR: expose vkCreateAndroidSurfaceKHR in volk + android surface structs.
    target_compile_definitions(ZVulkan PUBLIC VK_USE_PLATFORM_ANDROID_KHR)
    # volk dlopens libvulkan via libdl; android/log for diagnostics. Deliberately NO link to 'vulkan'.
    target_link_libraries(ZVulkan PUBLIC dl log android)
endif()
# Windows: vulkaninstance.h defines VK_USE_PLATFORM_WIN32_KHR itself and volk loads vulkan-1.dll.
