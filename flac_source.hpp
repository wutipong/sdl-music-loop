#pragma once
#include "pcm_source.hpp"
#include <memory>
#include <filesystem>

#include "dr_flac.h"

class FlacSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenFLAC(const std::filesystem::path &path);

  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest);

  virtual uint64_t FrameCount() const;

  ~FlacSource();

private:
  drflac *pFlac;
};
