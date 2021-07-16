#pragma once
#include "pcm_source.hpp"
#include <memory>
#include <string>

#include "dr_mp3.h"

class Mp3Source : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenMP3(std::string &path);

  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest);

  virtual uint64_t FrameCount() const;

  ~Mp3Source();

private:
  std::unique_ptr<drmp3> mp3;
  uint64_t frameCount;
};
