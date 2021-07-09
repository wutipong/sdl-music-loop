#include "ogg_source.hpp"
#include <stdexcept>

std::unique_ptr<PCMSource> OggSource::OpenOGG(const std::string& path) {
  OggVorbis_File vf;
  if (ov_fopen(path.c_str(), &vf) != 0) {
    throw std::invalid_argument("cannot read the file.");
  }
  vorbis_info * info = ov_info(&vf, -1);
  if (!ValidateAudioSpec(info->channels, info->rate)) {
    throw std::invalid_argument("unsupported audio file.");
  }

  auto output = new OggSource;
  output->size = ov_pcm_total(&vf, -1);
  output->file = std::move(vf);

  return std::unique_ptr<PCMSource>(output);
}
void OggSource::FillBuffer(const uint64_t& position, const uint64_t& count,
    SampleBuffer& buffer, const uint64_t& dest) {
  ov_pcm_seek(&file, position);
  int bitstream = 0;

  char *stream = reinterpret_cast<char *>(buffer.SampleData(dest));
  uint64_t sizeInBytes = count * sizeof(Frame);

  for (long readTotal = 0; readTotal < sizeInBytes;) {
    readTotal += ov_read(&file, stream + readTotal, sizeInBytes - readTotal, 0,
                        2, 1, &bitstream);
  }
}

uint64_t OggSource::FrameCount() const { return size; }

OggSource::~OggSource() { ov_clear(&file); }