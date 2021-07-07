#include "music.hpp"

Music::Music(BufferedSource &&source) : source(source) {
  loopStart = 0;
  loopEnd = source.FrameCount();
  current = 0;
}

void Music::FillBuffer(QueueBuffer &buffer) {
  if (current >= source.FrameCount())
    return;

  for (size_t bufferIndex = 0; bufferIndex < buffer.size();) {

    auto sourceAvailable = loopEnd - current;
    auto bufferRemain = buffer.size() - bufferIndex;

    if (sourceAvailable > bufferRemain) {
      source.FillBuffer(buffer, current, bufferRemain, bufferIndex);
      current += bufferRemain;
      return;
    }

    source.FillBuffer(buffer, current, sourceAvailable, bufferIndex);
    current += sourceAvailable;
    if (current >= loopEnd) {
      current = loopStart;
    }
    bufferIndex += sourceAvailable;
  }
}
