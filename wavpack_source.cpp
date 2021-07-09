#include "wavpack_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource> WavpackSource::OpenWV(std::string &path) {
  auto *output = new WavpackSource;
  output->context = WavpackOpenFileInput(path.c_str(), nullptr, 0, 0);

  auto ctx = output->context;
  if (!ctx) {
    delete output;
    throw std::invalid_argument("cannot read the file.");
  }

  auto channels = WavpackGetNumChannels(ctx);
  auto freq = WavpackGetSampleRate(ctx);

  if (!ValidateAudioSpec(channels, freq)) {
    WavpackCloseFile(ctx);
    throw std::invalid_argument("unsupported audio file.");
  }

  return std::unique_ptr<PCMSource>(output);
}

void WavpackSource::FillBuffer(QueueBuffer &buffer, const uint64_t &position,
                               const uint64_t &count, const uint64_t &dest) {
  WavpackSeekSample64(context, position);

  std::vector<int32_t> wvBuffer;
  wvBuffer.resize(count * Channels);
  WavpackUnpackSamples(context, wvBuffer.data(), count);

  // trim off the 16bit padding of each sample created by Wavpack.
  // each frames are consisted of 2 16-bit samples, one for left and another for
  // right channel.
  int16_t *ptr = reinterpret_cast<int16_t *>(buffer.data() + dest);
  for (int i = 0; i < wvBuffer.size(); i++) {
    ptr[i] = wvBuffer[i];
  }
}

uint64_t WavpackSource::FrameCount() const {
  return WavpackGetNumSamples64(context);
}

WavpackSource::~WavpackSource() { WavpackCloseFile(context); }
