#include "wave_source.hpp"
#include <stdexcept>

WaveSource WaveSource::Open(const std::string &path) {
  WaveSource output{};
  Uint8 *wav_buffer;
  SDL_AudioSpec wav_spec;
  Uint32 wav_length;

  if (SDL_LoadWAV(path.c_str(), &wav_spec, &wav_buffer, &wav_length) ==
      nullptr) {
    throw std::invalid_argument("cannot read the file.");
  }

  if (!ValidateAudioSpec(wav_spec)) {
    SDL_FreeWAV(wav_buffer);

    throw std::invalid_argument("unsupported audio file.");
  }

  output.wav_buffer =
      std::shared_ptr<Uint8>(wav_buffer, [](auto p) { SDL_FreeWAV(p); });
  output.wav_length = wav_length;
  output.wav_spec = wav_spec;

  return output;
}
