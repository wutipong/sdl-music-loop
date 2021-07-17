#pragma once
#include <SDL2/SDL.h>
#include <array>

class Music;

typedef float Sample;
typedef uint64_t Frame;

constexpr int SamplingRate = 44'100;
constexpr int Channels = 2;
constexpr SDL_AudioFormat Format = AUDIO_F32;
constexpr int BufferFrameCount = 1024;

void OpenAudioDevice();
void CloseAudioDevice();
void PrintAudioDeviceInfo();

bool ValidateAudioSpec(const SDL_AudioSpec &spec);
bool ValidateAudioSpec(const int &channels, const int &freq);

bool IsAudioNeedToQueue();
void QueueAudio(Music &src);

void PlayAudio();
void PauseAudio();
void ClearAudio();
