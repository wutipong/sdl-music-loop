#include "sample_buffer.hpp"

SampleBuffer::SampleBuffer(const uint64_t &size) : sampleSize(size) {
  data = std::unique_ptr<Frame[]>(new Frame[size]);
}

Sample *SampleBuffer::SampleData(const uint64_t &framePosition) {
  return reinterpret_cast<Sample *>(data.get() + framePosition);
}
Frame *SampleBuffer::FrameData(const uint64_t &framePosition) {
  return data.get() + framePosition;
}

uint64_t SampleBuffer::SampleDataSize() const {
  return (sampleSize / sizeof(Sample)) * sizeof(Frame);
}

uint64_t SampleBuffer::FrameDataSize() const { return sampleSize; }

uint64_t SampleBuffer::ByteSize() const { return sampleSize * sizeof(Frame); }
