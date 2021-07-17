#pragma once
#include "dr_mp3.h"
#include "pcm_source.hpp"
#include <filesystem>
#include <memory>
#include <string>

class Mp3Source : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenMP3(const std::filesystem::path &path);

  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest);

  virtual uint64_t FrameCount() const;
  Mp3Source(){};
  ~Mp3Source();

private:
  std::unique_ptr<drmp3> mp3 = std::make_unique<drmp3>();
  uint64_t frameCount;

  Mp3Source(const Mp3Source &other) {}
  Mp3Source &operator=(const Mp3Source &other) {}
};
