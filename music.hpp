#pragma once
#include "audio_device.hpp"
#include "buffered_source.hpp"

class Music {
public:
  Music(){};
  Music(BufferedSource &&source);

  void FillBuffer(QueueBuffer &buffer);

  size_t loopStart{0};
  size_t loopEnd{0};
  size_t current{0};

private:
  BufferedSource source;
};
