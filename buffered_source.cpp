#include "buffered_source.hpp"
#include <stdexcept>

BufferedSource::BufferedSource(const uint64_t& count): count(count) {
  data = std::unique_ptr<Frame[]>(new Frame[count]);
}

std::unique_ptr<PCMSource>
BufferedSource::OpenWAV(const std::filesystem::path &path) {
  Uint8 *buffer;
  SDL_AudioSpec spec;
  Uint32 length;

  if (SDL_LoadWAV(path.u8string().c_str(), &spec, &buffer, &length) ==
      nullptr) {
    throw std::invalid_argument("cannot read the file.");
  }

  if (!ValidateAudioSpec(spec)) {
    SDL_FreeWAV(buffer);

    throw std::invalid_argument("unsupported audio file.");
  }

  auto src = reinterpret_cast<Frame *>(buffer);
  auto frameCount = length / sizeof(Frame);

  auto output = std::make_unique<BufferedSource>(frameCount);

  /* Copy PCM data from the read buffer into the output.*/
  std::copy_n(src, frameCount, output->data.get());
  SDL_FreeWAV(buffer);

  return output;
}

void BufferedSource::FillBuffer(const uint64_t &position, const uint64_t &count,
                                SampleBuffer &buffer, const uint64_t &dest) {
  if (FrameCount() == 0)
    return;

  // Since we buffer the whole PCM data, we can just copy the required samples
  // into the buffer.
  std::copy_n(data.get() + position, count, buffer.FrameData(dest));
}
