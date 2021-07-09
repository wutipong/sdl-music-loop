#pragma once

#include "audio_device.hpp"
#include "sample_buffer.hpp"

class PCMSource {
public:
  virtual void FillBuffer(SampleBuffer &buffer, const uint64_t &position,
                          const uint64_t &count, const uint64_t &dest) = 0;

  virtual uint64_t FrameCount() const = 0;
};
