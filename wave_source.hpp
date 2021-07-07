#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "pcm_source.hpp"

class WaveSource {
public:
  static WaveSource Open(const std::string &path);

  void FillBuffer(QueueBuffer &buffer);

private:
  std::shared_ptr<Uint8> buffer;
  SDL_AudioSpec spec;
  Uint32 length{0};

  size_t position{0};
};
