#include "music.hpp"

Music::Music(std::unique_ptr<PCMSource> &&s) : source(std::move(s)) {
  loopStart = 0;
  loopEnd = source->FrameCount();
  current = 0;
}

void Music::FillBuffer(SampleBuffer &buffer) {
  if (source == nullptr || current >= source->FrameCount())
    return;

  for (size_t bufferIndex = 0; bufferIndex < buffer.FrameDataSize();) {

    auto sourceAvailable = loopEnd - current;
    auto bufferRemain = buffer.FrameDataSize() - bufferIndex;

    if (sourceAvailable > bufferRemain) {
      source->FillBuffer(buffer, current, bufferRemain, bufferIndex);
      current += bufferRemain;
      return;
    }

    source->FillBuffer(buffer, current, sourceAvailable, bufferIndex);
    current += sourceAvailable;
    if (current >= loopEnd) {
      current = loopStart;
    }
    bufferIndex += sourceAvailable;
  }
}
