/*///-------------------------------------------------------------------------------------------------------------------
Copyright(c) 2021 iNVOXRecords

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
/*///-------------------------------------------------------------------------------------------------------------------

#pragma once

#include <juce_core/juce_core.h>
#include <cstdint>

using namespace juce;
using uint32 = std::uint32_t;

namespace iNVOXRecords::processor {
//----------------------------------------------------------------------------------------------------------------------
// CircularBuffer class
//----------------------------------------------------------------------------------------------------------------------
template <class T>
class CircularBuffer
{
public:
  // constructor
  CircularBuffer() noexcept {}

  // override
  T& operator[](uint32 index) { return getFront(index); }

  // common method
  void clear()
  {
    length = readPoint = writePoint = 0;
    buffer.clear(static_cast<size_t>(capacity));
  }

  void push(const T& data)
  {
    forward();
    if (bufferSize > length && !isFull()) length++;
    buffer[writePoint] = data;
    writePoint = normIndex(writePoint + 1);
  }

  T& getFront(uint32 index)
  {
    #if JUCE_DEBUG
    jassert(index < bufferSize);
    #endif

    return buffer.get()[normIndex(readPoint + index)];
  }

  T& getBack(uint32 index)
  {
    #if JUCE_DEBUG
    jassert(index < bufferSize);
    #endif

    return buffer.get()[normIndex(readPoint + length - index - 1)];
  }

  bool isFull()
  {
    return bufferSize == length;
  }

  bool isEmpty()
  {
    return length == 0;
  }

  // getter
  uint32 getBufferSize()
  {
    return bufferSize;
  }

  uint32 getLength()
  {
    return length;
  }

  // setter
  void setBufferSize(uint32 size)
  {
    capacity = nextPowerOfTwo(size);
    buffer.malloc(capacity);
    bufferSize = size;
    clear();
  }

private:
  HeapBlock<T> buffer;
  uint32 capacity = 0;
  uint32 bufferSize = 0;
  uint32 length = 0;
  uint32 readPoint = 0;
  uint32 writePoint = 0;

  // helper
  uint32 normIndex(uint32 index) { return index & (capacity - 1); }

  void forward()
  {
    if (length == bufferSize) {
      readPoint = normIndex(readPoint + 1);
    }
  }
};

} // namespace iNVOXRecords::processor