#pragma once

#include "audio_device.hpp"
#include "pcm_source.hpp"
#include "sample_buffer.hpp"
#include <memory>
#include <string>
#include <vorbis/vorbisfile.h>

class OggSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenOGG(const std::string &path);
  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest) override;

  virtual uint64_t FrameCount() const override;

  virtual ~OggSource();

private:
  OggVorbis_File file;
  uint64_t size;
};
