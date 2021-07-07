#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

#include "pcm_source.hpp"

class BufferedSource {
public:
  static BufferedSource OpenWAV(const std::string &path);

  void FillBuffer(QueueBuffer &buffer, const size_t &position,
                  const size_t &count, const size_t &dest);

  int FrameCount() const { return frames.size(); }

private:
  std::vector<Frame> frames;
  std::vector<Frame>::iterator iter;
  bool valid{false};
};
