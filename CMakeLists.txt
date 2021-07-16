cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

# set the project name
project(sdl-music-loop)

# add the executable
add_executable(sdl-music-loop 
	"main.cpp" "io_util.hpp"
	"imgui_impl/imgui_impl_sdl.h" 
	"imgui_impl/imgui_impl_sdl.cpp"
	"imgui_impl/imgui_impl_opengl3.h" 
	"imgui_impl/imgui_impl_opengl3.cpp" 
	"imgui-filebrowser/imfilebrowser.h"
	"scene.hpp" 
	"buffered_source.hpp" "buffered_source.cpp" 
	"pcm_source.hpp" 
	"audio_device.hpp" "audio_device.cpp" 
	"music.hpp" "music.cpp" 
	"wavpack_source.hpp" "wavpack_source.cpp" 
	"sample_buffer.hpp" "sample_buffer.cpp" 
	"ogg_source.hpp" "ogg_source.cpp" 
	"mixer_scene.hpp" "mixer_scene.cpp" 
	"menu_scene.hpp" "menu_scene.cpp" 
	"audio_device_scene.cpp" "audio_device_scene.hpp" 
	"flac_source.hpp" "flac_source.cpp" "mp3_source.hpp" "mp3_source.cpp")

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(wavpack CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)

find_path(DRLIBS_INCLUDE_DIRS "dr_flac.h")

target_link_libraries(sdl-music-loop PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
	SDL2::SDL2_mixer
    imgui::imgui 
    unofficial::gl3w::gl3w 
    spdlog::spdlog 
    spdlog::spdlog_header_only
    fmt::fmt
	WavPack::wavpack
	Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile)

target_include_directories(sdl-music-loop PRIVATE ${DRLIBS_INCLUDE_DIRS})

target_compile_features(sdl-music-loop PRIVATE cxx_std_17)
