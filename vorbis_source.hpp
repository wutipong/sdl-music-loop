#pragma once

#include "audio_device.hpp"
#include "pcm_source.hpp"
#include "sample_buffer.hpp"
#include <filesystem>
#include <memory>
#include <vorbis/vorbisfile.h>

class VorbisSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenOGG(const std::filesystem::path &path);
  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest) override;

  virtual uint64_t FrameCount() const override;

  VorbisSource(){};
  virtual ~VorbisSource();

private:
  VorbisSource(const VorbisSource &other) {}
  VorbisSource &operator=(const VorbisSource &other) {}

  OggVorbis_File file{};
  uint64_t count;
};
