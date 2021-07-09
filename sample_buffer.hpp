#pragma once

#include "audio_device.hpp"
#include <SDL2/SDL.h>
#include <memory>

class SampleBuffer {
public:
  SampleBuffer(const uint64_t &sampleCount);

  Sample *SampleData(const uint64_t &framePosition = 0);
  Frame *FrameData(const uint64_t &framePosition = 0);

  uint64_t SampleDataSize() const;
  uint64_t FrameDataSize() const;
  uint64_t ByteSize() const;

private:
  std::unique_ptr<Frame[]> data;
  uint64_t sampleSize;
};
