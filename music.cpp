#include "music.hpp"

Music::Music(BufferedSource &&source) : source(source) {
  loopStart = 0;
  loopEnd = source.FrameCount();
  current = 0;
}

void Music::FillBuffer(QueueBuffer &buffer) {
  if (source.FrameCount() == 0)
    return;

  for (auto bufferIndex = 0; bufferIndex < buffer.size();) {

    auto dist = loopEnd - current;
    auto queueDistance = buffer.size() - bufferIndex;

    if (dist > queueDistance) {
      source.FillBuffer(buffer, current, queueDistance, bufferIndex);
      current += queueDistance;
      return;
    } else {
      source.FillBuffer(buffer, current, dist, bufferIndex);
      current += dist;
      if (current >= loopEnd) {
        current = loopStart;
      }
      bufferIndex += dist;
    }
  }
}
