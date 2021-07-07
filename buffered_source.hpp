#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

#include "pcm_source.hpp"

class BufferedSource {
public:
  static BufferedSource OpenWAV(const std::string &path);

  void FillBuffer(QueueBuffer &buffer, const uint64_t &position,
                  const uint64_t &count, const uint64_t &dest);

  uint64_t FrameCount() const { return frames.size(); }

private:
  std::vector<Frame> frames;
  std::vector<Frame>::iterator iter;
  bool valid{false};
};
