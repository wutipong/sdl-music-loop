#include "music.hpp"
#include "buffered_source.hpp"
#include "wavpack_source.hpp"
#include "ogg_source.hpp"
#include "flac_source.hpp"
#include "mp3_source.hpp"

Music::Music(std::unique_ptr<PCMSource> &&s) : source(std::move(s)) {
  loopStart = 0;
  loopEnd = source->FrameCount();
  current = 0;
}

void Music::FillBuffer(SampleBuffer &buffer) {
  if (source == nullptr || current >= source->FrameCount()) {
    std::fill_n(buffer.FrameData(), buffer.FrameDataSize(), 0);
    return;
  }

  for (size_t bufferIndex = 0; bufferIndex < buffer.FrameDataSize();) {
    auto sourceAvailable = loopEnd - current;
    auto bufferRemain = buffer.FrameDataSize() - bufferIndex;

    if (sourceAvailable > bufferRemain) {
      source->FillBuffer(current, bufferRemain, buffer, bufferIndex);
      current += bufferRemain;
      return;
    }

    source->FillBuffer(current, sourceAvailable, buffer, bufferIndex);
    current += sourceAvailable;
    if (current >= loopEnd) {
      current = loopStart;
    }
    bufferIndex += sourceAvailable;
  }
}

Music Music::Open(const std::filesystem::path &path) {
  std::unique_ptr<PCMSource> source;
  auto ext = path.extension().string();
  if (ext == ".wav") {
    source = BufferedSource::OpenWAV(path.string());
  } else if (ext == ".wv") {
    source = WavpackSource::OpenWV(path.string());
  } else if (ext == ".ogg") {
    source = OggSource::OpenOGG(path.string());
  } else if (ext == ".flac") {
    source = FlacSource::OpenFLAC(path.string());
  } else if (ext == ".mp3") {
    source = Mp3Source::OpenMP3(path.string());
  }

  return Music(std::move(source));
}
