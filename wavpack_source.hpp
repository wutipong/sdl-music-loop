#pragma once
#include "pcm_source.hpp"
#include <memory>
#include <string>
#include <wavpack.h>

class WavpackSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenWV(std::string &path);

  virtual void FillBuffer(const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest);

  virtual uint64_t FrameCount() const;

  ~WavpackSource();

private:
  WavpackContext *context;
};
