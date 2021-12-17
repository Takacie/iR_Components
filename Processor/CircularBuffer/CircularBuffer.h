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
class CircularBuffer {
public:
  // constructor
  CircularBuffer() noexcept {}

  // override
  T& operator[](uint32 index) { return getFront(index); }

  // common method
  void clear()
  {
    length = read_point = write_point = 0;
    buffer.clear(static_cast<size_t>(capacity));
  }

  void push(const T& data)
  {
    forward();
    if (buffer_size > length && !isFull()) length++;
    buffer[write_point] = data;
    write_point = normIndex(write_point + 1);
  }

  T& getFront(uint32 index)
  {
    jassert(index < buffer_size);

    return buffer.get()[normIndex(read_point + index)];
  }

  T& getBack(uint32 index)
  {
    jassert(index < buffer_size);

    return buffer.get()[normIndex(read_point + length - index - 1)];
  }

  bool isFull()
  { 
    return buffer_size == length;
  }

  bool isEmpty()
  {
    return length == 0;
  }

  // getter
  uint32 getBufferSize()
  {
    return buffer_size;
  }

  uint32 getLength()
  {
    return length;
  }

  // setter
  void setBufferSize(uint32 size) {
    capacity = nextPowerOfTwo(size);
    buffer.malloc(capacity);
    buffer_size = size;
    clear();
  }

private:
  HeapBlock<T> buffer;
  uint32 capacity = 0;
  uint32 buffer_size = 0;
  uint32 length = 0;
  uint32 read_point = 0;
  uint32 write_point = 0;

  // helper
  uint32 normIndex(uint32 index) { return index & (capacity - 1); }

  void forward() 
  { 
    if (length == buffer_size) {
      read_point = normIndex(read_point + 1);
    }
  }
};

} // namespace iNVOXRecords::processor