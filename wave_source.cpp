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

  auto src = reinterpret_cast<Frame *>(buffer);
  auto frameCount = length / BytesPerFrame;

  output.frames = std::vector<Frame>(src, src + frameCount);

  SDL_FreeWAV(buffer);

  return output;
}

void WaveSource::FillBuffer(QueueBuffer &queueBuffer) {
  if (frames.empty())
    return;

  if (!valid) {
    iter = frames.begin();
    valid = true;
  }

  for (auto queue_iter = queueBuffer.begin();
       queue_iter != queueBuffer.end();) {

    auto dist = frames.end() - iter;
    auto queueDistance = queueBuffer.end() - queue_iter;

    if (dist > queueDistance) {
      std::copy_n(iter, queueDistance, queue_iter);
      iter += queueDistance;
      return;
    } else {
      std::copy_n(iter, dist, queue_iter);
      iter += dist;
      queue_iter += dist;
    }
  }
}
