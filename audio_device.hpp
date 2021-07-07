#pragma once
#include <SDL2/SDL.h>
#include <array>

class WaveSource;

constexpr int SamplingRate = 44'100;
constexpr int Channels = 2;
constexpr SDL_AudioFormat Format = AUDIO_S16;
constexpr int BufferFrameCount = 1024;

constexpr size_t QueueBufferSize =
    BufferFrameCount * Channels * (SDL_AUDIO_BITSIZE(Format) / 8);

typedef std::array<Uint8, QueueBufferSize> QueueBuffer;

void OpenAudioDevice();
void CloseAudioDevice();
void PrintAudioDeviceInfo();

bool ValidateAudioSpec(const SDL_AudioSpec &spec);

bool IsAudioNeedToQueue();
void QueueAudio(WaveSource &src);

void PlayAudio();
void PauseAudio();
void ClearAudio();
