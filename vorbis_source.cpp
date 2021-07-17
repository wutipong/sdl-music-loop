#include "vorbis_source.hpp"
#include <stdexcept>

std::unique_ptr<PCMSource>
VorbisSource::OpenOGG(const std::filesystem::path &path) {
  auto output = std::make_unique<VorbisSource>();

  OggVorbis_File &vf = output->file;
  if (ov_fopen(path.u8string().c_str(), &vf) != 0) {
    throw std::invalid_argument("cannot read the file.");
  }
  vorbis_info *info = ov_info(&vf, -1);
  if (!ValidateAudioSpec(info->channels, info->rate)) {
    throw std::invalid_argument("unsupported audio file.");
  }

  output->count = ov_pcm_total(&vf, -1);

  return output;
}
void VorbisSource::FillBuffer(const uint64_t &position, const uint64_t &count,
                              SampleBuffer &buffer, const uint64_t &dest) {
  ov_pcm_seek(&file, position);
  int bitstream = 0;

  /* `ov_read_float` might read less than the supplied parameter from the
   * stream. Use a loop to keep reading uptil the buffer is filled. */

  for (long readTotal = 0; readTotal < count;) {
    float **pcm{};

    /* `ov_read_float` returns multiple stream where each one contains a
     * channel. SDL playbacks require interleave data, so we need to build one.
     */
    auto samples_read =
        ov_read_float(&file, &pcm, count - readTotal, &bitstream);

    for (int i = 0; i < samples_read; i++) {
      float *left = buffer.SampleData(dest + readTotal + i);
      float *right = left + 1;

      *left = pcm[0][i];
      *right = pcm[1][i];
    }

    readTotal += samples_read;
  }
}

uint64_t VorbisSource::FrameCount() const { return count; }

VorbisSource::~VorbisSource() { ov_clear(&file); }
