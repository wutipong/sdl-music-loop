#include "wavpack_source.hpp"
#include <stdexcept>

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

  // TODO: Wavpack Sample is always in 32 bit where we need 16-bit here.
  WavpackUnpackSamples(context, buffer.data() + dest, count);
}

uint64_t WavpackSource::FrameCount() const {
  return WavpackGetNumSamples64(context);
}

WavpackSource::~WavpackSource() { WavpackCloseFile(context); }
