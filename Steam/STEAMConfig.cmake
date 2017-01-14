set(VULKAN_INCLUDE_DIRS $ENV{STEAM_SDK_PATH}/public)
set(VULKAN_LIBRARY_DIRS $ENV{STEAM_SDK_PATH}/redistributable_bin/win64)
set(VULKAN_LIBRARIES debug steam_api64 optimized steam_api64)

mark_as_advanced(STEAM)