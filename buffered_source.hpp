#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <filesystem>
#include <vector>

#include "pcm_source.hpp"
#include <memory>

class BufferedSource : public PCMSource {
public:
  static std::unique_ptr<PCMSource> OpenWAV(const std::filesystem::path &path);

  BufferedSource(const uint64_t &count);

  virtual void FillBuffer( const uint64_t &position, const uint64_t &count,
                          SampleBuffer &buffer, const uint64_t &dest) override;

  virtual uint64_t FrameCount() const override { return count; }

private:
  std::unique_ptr<Frame[]> data;
  uint64_t count{0};
};
