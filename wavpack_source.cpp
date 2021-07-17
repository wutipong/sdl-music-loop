#include "wavpack_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource>
WavpackSource::OpenWV(const std::filesystem::path &path) {
  auto output = std::make_unique<WavpackSource>();
  output->context =
      WavpackOpenFileInput(path.u8string().c_str(), nullptr, 0, 0);

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

void WavpackSource::FillBuffer(const uint64_t &position, const uint64_t &count,
                               SampleBuffer &buffer, const uint64_t &dest) {
  WavpackSeekSample64(context, position);

  std::vector<int32_t> wvBuffer;
  wvBuffer.resize(count * Channels);

  /* Wavpack will decode the file into a stream of 32-bit data, either
   * floating-point or integer. Since we reject any unmatched file format, this
   * is guaranteed to be 32-bit floating point stream. We can fill the buffer
   * directly.
   *
   * If the integer format using less than 32-bit sample size is in used, then
   * we need to remove the padding out of the sample stream.
   */

  WavpackUnpackSamples(context,
                       reinterpret_cast<int32_t *>(buffer.SampleData(dest)),
                       static_cast<uint32_t>(count));
}

uint64_t WavpackSource::FrameCount() const {
  return WavpackGetNumSamples64(context);
}

WavpackSource::~WavpackSource() { WavpackCloseFile(context); }
