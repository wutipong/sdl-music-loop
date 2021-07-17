#include "sample_buffer.hpp"

SampleBuffer::SampleBuffer(const uint64_t &count) : count(count) {
  data = std::unique_ptr<Frame[]>(new Frame[count]);
}

Sample *SampleBuffer::SampleData(const uint64_t &framePosition) {
  return reinterpret_cast<Sample *>(FrameData(framePosition));
}
Frame *SampleBuffer::FrameData(const uint64_t &framePosition) {
  return data.get() + framePosition;
}

uint64_t SampleBuffer::FrameCount() const { return count; }

uint64_t SampleBuffer::ByteSize() const { return count * sizeof(Frame); }
