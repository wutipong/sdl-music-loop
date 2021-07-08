#pragma once

#include "audio_device.hpp"

class PCMSource {
public:
  virtual void FillBuffer(QueueBuffer &buffer, const uint64_t &position,
                          const uint64_t &count, const uint64_t &dest) = 0;

  virtual uint64_t FrameCount() const = 0;
};
