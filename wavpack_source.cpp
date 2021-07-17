#include "wavpack_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource>
WavpackSource::OpenWV(const std::filesystem::path &path) {
  auto output = std::make_unique<WavpackSource>();
  output->context = WavpackOpenFileInput(path.u8string().c_str(), nullptr, 0, 0);

  auto &ctx = output->context;
  if (!ctx) {
    throw std::invalid_argument("cannot read the file.");
  }

  auto channels = WavpackGetNumChannels(ctx);
  auto freq = WavpackGetSampleRate(ctx);

  if (!ValidateAudioSpec(channels, freq)) {
    WavpackCloseFile(ctx);
    throw std::invalid_argument("unsupported audio file.");
  }

  return output;
}

void WavpackSource::FillBuffer( const uint64_t &position, const uint64_t &count,
                               SampleBuffer &buffer, const uint64_t &dest) {
  WavpackSeekSample64(context, position);
  
  std::vector<int32_t> wvBuffer;
  wvBuffer.resize(count * Channels);
  WavpackUnpackSamples(context, reinterpret_cast<int32_t*>(buffer.SampleData(dest)),
                       static_cast<uint32_t>(count));
}

uint64_t WavpackSource::FrameCount() const {
  return WavpackGetNumSamples64(context);
}

WavpackSource::~WavpackSource() { WavpackCloseFile(context); }
