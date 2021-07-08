#pragma once
#include <SDL2/SDL.h>
#include <array>

class Music;

typedef int32_t Frame;
constexpr int SamplingRate = 44'100;
constexpr int Channels = 2;
constexpr SDL_AudioFormat Format = AUDIO_S16;
constexpr int BufferFrameCount = 1024;
constexpr size_t BytesPerFrame = sizeof(Frame);
constexpr size_t QueueBufferSize = BufferFrameCount * BytesPerFrame;

typedef std::array<Frame, BufferFrameCount> QueueBuffer;

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
