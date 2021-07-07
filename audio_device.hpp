#pragma once
#include <SDL2/SDL.h>

constexpr int SamplingRate = 44'100;
constexpr int Channels = 2;
constexpr SDL_AudioFormat Format = AUDIO_S16;
constexpr int BufferSize = 1024;

void OpenAudioDevice();
void CloseAudioDevice();
void PrintAudioDeviceInfo();

bool ValidateAudioSpec(const SDL_AudioSpec &spec);