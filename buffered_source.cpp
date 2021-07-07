#include "buffered_source.hpp"
#include <stdexcept>

BufferedSource BufferedSource::OpenWAV(const std::string &path) {
  BufferedSource output{};
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

  auto src = reinterpret_cast<Frame *>(buffer);
  auto frameCount = length / BytesPerFrame;

  output.frames = std::vector<Frame>(src, src + frameCount);

  SDL_FreeWAV(buffer);

  return output;
}

void BufferedSource::FillBuffer(QueueBuffer &buffer, const size_t &position,
                                const size_t &count, const size_t &dest) {
  if (frames.empty())
    return;

  std::copy_n(frames.begin() + position, count, buffer.begin() + dest);
}
