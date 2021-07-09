#include "audio_device.hpp"
#include "music.hpp"
#include <array>
#include <spdlog/spdlog.h>
#include "sample_buffer.hpp"

namespace {
SDL_AudioDeviceID deviceId;
SDL_AudioSpec audioSpec;
} // namespace

void OpenAudioDevice() {
  SDL_AudioSpec desiredSpec;
  desiredSpec.freq = SamplingRate;
  desiredSpec.format = Format;
  desiredSpec.channels = Channels;
  desiredSpec.samples = BufferFrameCount;
  desiredSpec.callback = nullptr;

  deviceId = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &audioSpec, 0);
}
void CloseAudioDevice() { SDL_CloseAudioDevice(deviceId); }

void PrintAudioDeviceInfo() {
  spdlog::info("Audio Device Information.");
  spdlog::info("\tChannels#: {}", audioSpec.channels);
  spdlog::info(
      "\tSample Format: {} {} {}-bit {}",
      SDL_AUDIO_ISSIGNED(audioSpec.format) ? "Signed" : "Unsigned",
      SDL_AUDIO_ISFLOAT(audioSpec.format) ? "Floating Point" : "Integer",
      SDL_AUDIO_BITSIZE(audioSpec.format),
      SDL_AUDIO_ISBIGENDIAN(audioSpec.format) ? "Big Endian" : "Little Endian");

  spdlog::info("\tSampling Rate: {}KHz", audioSpec.freq / 1000.0);
  spdlog::info("\tBuffer Size: {} frames", audioSpec.samples);
}

bool ValidateAudioSpec(const SDL_AudioSpec &spec) {
  if (spec.format != Format)
    return false;
  if (spec.freq != SamplingRate)
    return false;
  if (spec.channels != Channels)
    return false;

  return true;
}

bool ValidateAudioSpec(const int &channels, const int &freq) {
  if (freq != SamplingRate)
    return false;
  if (channels != Channels)
    return false;

  return true;
}

bool IsAudioNeedToQueue() {
  constexpr size_t QueueBufferSize = BufferFrameCount * sizeof(Frame);

  return SDL_GetQueuedAudioSize(deviceId) < QueueBufferSize;
}

void QueueAudio(Music &m) {
  SampleBuffer buffer(BufferFrameCount);
  m.FillBuffer(buffer);
  SDL_QueueAudio(deviceId, buffer.SampleData(), buffer.ByteSize());
}

void PlayAudio() { SDL_PauseAudioDevice(deviceId, 0); }
void PauseAudio() { SDL_PauseAudioDevice(deviceId, 1); }
void ClearAudio() { SDL_ClearQueuedAudio(deviceId); }
