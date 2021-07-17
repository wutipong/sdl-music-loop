#include "flac_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource>
FlacSource::OpenFLAC(const std::filesystem::path &path) {
  auto pFlac = drflac_open_file(path.u8string().c_str());
  if (pFlac == nullptr) {
    throw std::invalid_argument("cannot read the file.");
  }

  auto output = std::make_unique<FlacSource>(pFlac);

  if (!ValidateAudioSpec(pFlac->channels, pFlac->sampleRate)) {
    drflac_close(pFlac);
    throw std::invalid_argument("unsupported audio file.");
  }

  return output;
}

void FlacSource::FillBuffer(const uint64_t &position, const uint64_t &count,
                            SampleBuffer &buffer, const uint64_t &dest) {
  if (pFlac == nullptr) {
    return;
  }

  drflac_seek_to_pcm_frame(pFlac, position);

  /* `drflac_read_pcm_frames_f32` might read less than the supplied parameter
   * from the stream. Use a loop to keep reading uptil the buffer is filled. */
  for (uint64_t frameRead = 0; frameRead < count;) {
    frameRead += drflac_read_pcm_frames_f32(pFlac, count - frameRead,
                                            buffer.SampleData(frameRead));
  }
}

uint64_t FlacSource::FrameCount() const {
  return pFlac != nullptr ? pFlac->totalPCMFrameCount : 0;
}

FlacSource::~FlacSource() { drflac_close(pFlac); }

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"