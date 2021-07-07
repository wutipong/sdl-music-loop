#pragma once
#include "audio_device.hpp"
#include "buffered_source.hpp"

class Music {
public:
  Music(){};
  Music(BufferedSource &&source);

  void FillBuffer(QueueBuffer &buffer);

  uint64_t loopStart{0};
  uint64_t loopEnd{0};
  uint64_t current{0};

  uint64_t Duration() { return source.FrameCount(); }

private:
  BufferedSource source;
};
