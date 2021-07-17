#include "mp3_source.hpp"
#include <SDL2/SDL.h>
#include <limits>
#include <stdexcept>
#include <vector>

std::unique_ptr<PCMSource>
Mp3Source::OpenMP3(const std::filesystem::path &path) {
  auto output = std::make_unique<Mp3Source>();
  auto &mp3 = output->mp3;

  if (!drmp3_init_file(mp3.get(), path.u8string().c_str(), NULL)) {
    throw std::invalid_argument("cannot read the file.");
  }

  if (!ValidateAudioSpec(mp3->channels, mp3->sampleRate)) {
    drmp3_uninit(mp3.get());
    throw std::invalid_argument("unsupported audio file.");
  }

  output->frameCount = drmp3_get_pcm_frame_count(mp3.get());

  return output;
}

void Mp3Source::FillBuffer(const uint64_t &position, const uint64_t &count,
                           SampleBuffer &buffer, const uint64_t &dest) {

  drmp3_seek_to_pcm_frame(mp3.get(), position);

  uint64_t frameRead = 0;
  while (frameRead < count) {
    frameRead += drmp3_read_pcm_frames_f32(
        mp3.get(), count - frameRead,
        reinterpret_cast<float*>(buffer.FrameData(frameRead)));
  }
}

uint64_t Mp3Source::FrameCount() const { return frameCount; }

Mp3Source::~Mp3Source() { drmp3_uninit(mp3.get()); }

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"