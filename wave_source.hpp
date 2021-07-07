#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "pcm_source.hpp"

class WaveSource {
public:
  static WaveSource Open(const std::string &path);

private:
  std::shared_ptr<Uint8> wav_buffer;
  SDL_AudioSpec wav_spec;
  Uint32 wav_length;
};
