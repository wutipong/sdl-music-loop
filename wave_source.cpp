#include "wave_source.hpp"
#include <stdexcept>

WaveSource WaveSource::Open(const std::string &path) {
  WaveSource output{};
  Uint8 *buffer;
  SDL_AudioSpec spec;
  Uint32 length;

  if (SDL_LoadWAV(path.c_str(), &spec, &buffer, &length) == nullptr) {
    throw std::invalid_argument("cannot read the file.");
  }

  if (!ValidateAudioSpec(spec)) {
    SDL_FreeWAV(buffer);

    throw std::invalid_argument("unsupported audio file.");
  }

  output.buffer =
      std::shared_ptr<Uint8>(buffer, [](auto p) { SDL_FreeWAV(p); });
  output.length = length;
  output.spec = spec;

  return output;
}

void WaveSource::FillBuffer(QueueBuffer &queueBuffer) {
  if (length == 0)
    return;
  for (int cursor = 0; cursor < queueBuffer.size();) {
    auto targetAmount = queueBuffer.size() - cursor;
    auto available =
        length - position > targetAmount ? targetAmount : length - position;

    auto copyAmount = queueBuffer.size() - cursor > available
                          ? available
                          : queueBuffer.size() - cursor;

    std::copy(buffer.get() + position, buffer.get() + position + copyAmount,
              queueBuffer.data() + cursor);
    position += copyAmount;
    if (position >= length) {
      position = 0;
    }
    cursor += copyAmount;
  }
}
