# sdl-music-loop

`sdl-music-loop` is a music player demo which support loop point (loop_start-loopend). Currently, the following formats are support.

* MPEG Layer III audio (mp3) via [dr_mp3](https://github.com/mackron/dr_libs/blob/master/dr_mp3.h) library.
* FLAC via [dr_flac](https://github.com/mackron/dr_libs/blob/master/dr_flac.h).
* [OGG Vorbis](https://xiph.org/vorbis/).
* [Wavpack](https://www.wavpack.com/).
* Uncompressed PCM.

The audo file must meet the following specification.

* Sampling Rate: 44.1KHz.
* Chanels: 2.
* Sample format: 32-Bit Floating Point.

The demo contains 2 method of playing
* streaming into SDL2 audio device directly.
* streaming using SDL_mixer

## Usage.

Choose either SDL Audio Device or SDL_Mixer, then the player screen displays. 

![screenshot](https://user-images.githubusercontent.com/3213191/126045697-07434c20-f86a-457e-8156-9b8b940765b4.png)

Press "Open File" and choose an audio file. 

By default, "Loop Start" is set to the begining of the file, and "Loop End" is set to the end of the file. Both fields value are in sample time. 


Upon pressing "Play", the song will be playing from the begining of the file, when it reach the "Loop End" point, the program will seek back to the "Loop Start" point. The song will be played indefinitely until "Pause" button is pressed. 

"Reset" button will reset the cursor to the begining of the song.

## Build the project

### Using IDE

Visual Studio 2019 and Visual Studio Code supports CMake project directly, given that the right components are installed. Simply use `Open Folder` menu to open the project file inside.

### Manually configure
This project requires [cmake](https://cmake.org) to build.

* make sure to checkout the subprojects, [VCPKG](https://vcpkg.io/en/index.html) and [imgui-filebrowser](https://github.com/AirGuanZ/imgui-filebrowser).
* run `cmake <project directory>` inside an empty directory to configure the project. The configure step might take some time as it build the dependencies projects at the fast time it is configured. 
* run `cmake --build .` (`.` must be included in the command) to build the project.

## NOTICE
This project contains files taken from `Dear ImGui` project, which is also released under MIT license.
