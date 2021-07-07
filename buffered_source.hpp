#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

#include "pcm_source.hpp"

class BufferedSource {
public:
  static BufferedSource OpenWAV(const std::string &path);

  void FillBuffer(QueueBuffer &buffer);

  int FrameCount() const { return frames.size(); }

private:
  std::vector<Frame> frames;
  std::vector<Frame>::iterator iter;
  bool valid{false};
};
