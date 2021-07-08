#pragma once
#include "audio_device.hpp"
#include "pcm_source.hpp"
#include <memory>

class Music {
public:
  Music(){};
  Music(std::unique_ptr<PCMSource> &&s);

  void FillBuffer(QueueBuffer &buffer);

  uint64_t loopStart{0};
  uint64_t loopEnd{0};
  uint64_t current{0};

  uint64_t Duration() { return source->FrameCount(); }

private:
  std::unique_ptr<PCMSource> source;
};
