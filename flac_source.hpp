#pragma once
#include "pcm_source.hpp"
#include <filesystem>
#include <memory>

#include "dr_flac.h"

class FlacSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenFLAC(const std::filesystem::path &path);

  FlacSource(drflac *pFlac) : pFlac(pFlac){};
  ~FlacSource();

  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest);

  virtual uint64_t FrameCount() const;

private:
  drflac *pFlac = nullptr;

  FlacSource(const FlacSource &other) {}
  FlacSource &operator=(const FlacSource &other) {}
};
