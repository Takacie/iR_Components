#pragma once

#include "../CircularBuffer/iR_CircularBuffer.h"

namespace iNVOXRecords::processor {
//----------------------------------------------------------------------------------------------------------------------
// iR_RMSDetector class
//----------------------------------------------------------------------------------------------------------------------
class iR_RMSDetector {
public:
  // common method
  void addSample(float sample)
  {
    if (buffer.isFull()) {
      sum -= buffer[0];
    }
    sum += sample * sample;
    buffer.push(sample * sample);
  }

  // getter
  float getRMS()
  {
    return std::sqrt(sum / buffer.getLength());
  }

  // setter
  void setBufferSize(int size)
  {
    buffer.setBufferSize(size);
  }

private:
  iR_CircularBuffer<float> buffer;
  float sum = 0.0f;
};

} // namespace iNVOXRecords::processor