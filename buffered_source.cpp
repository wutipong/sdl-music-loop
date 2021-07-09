#include "buffered_source.hpp"
#include <stdexcept>

std::unique_ptr<PCMSource> BufferedSource::OpenWAV(const std::string &path) {
  auto output = new BufferedSource;
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
  auto frameCount = length / sizeof(Frame);

  output->frames = std::vector<Frame>(src, src + frameCount);

  SDL_FreeWAV(buffer);

  return std::unique_ptr<PCMSource>(output);
}

void BufferedSource::FillBuffer(SampleBuffer &buffer, const uint64_t &position,
                                const uint64_t &count, const uint64_t &dest) {
  if (frames.empty())
    return;

  std::copy_n(frames.begin() + position, count, buffer.FrameData(dest));
}
