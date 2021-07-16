#include "flac_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource> FlacSource::OpenFLAC(std::string &path) {
  auto *output = new FlacSource;
  output->pFlac = drflac_open_file(path.c_str());
  if (output->pFlac == NULL) {
    delete output;
    throw std::invalid_argument("cannot read the file.");
  }
 
  auto pFlac = output->pFlac;

  if (!ValidateAudioSpec(pFlac->channels, pFlac->sampleRate)) {
    drflac_close(pFlac);
    delete output;
    throw std::invalid_argument("unsupported audio file.");
  }

  return std::unique_ptr<PCMSource>(output);
}

void FlacSource::FillBuffer(const uint64_t &position, const uint64_t &count,
                               SampleBuffer &buffer, const uint64_t &dest) {

  drflac_seek_to_pcm_frame(pFlac, position);

  std::vector<int32_t> wvBuffer;
  wvBuffer.resize(count * Channels);

  drflac_read_pcm_frames_s16(
      pFlac, count, reinterpret_cast<drflac_int16 *>(buffer.FrameData()));
}

uint64_t FlacSource::FrameCount() const { return pFlac->totalPCMFrameCount; }

FlacSource::~FlacSource() { drflac_close(pFlac); }

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"