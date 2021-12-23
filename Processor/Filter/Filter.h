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

#include "../../Utility/Helper/Helper.h"
#include "juce_audio_processors/juce_audio_processors.h"

using namespace juce;
using namespace dsp;

namespace iNVOXRecords::processor {
//----------------------------------------------------------------------------------------------------------------------
// FilterType enum class
//----------------------------------------------------------------------------------------------------------------------
enum class FilterType
{
  LowCut,
  HighCut,
  Peak,
  LowShelf,
  HighShelf,
  BandPass,
  Notch
};

//----------------------------------------------------------------------------------------------------------------------
// IIRFilter class
//----------------------------------------------------------------------------------------------------------------------
class IIRFilter
{
public:
  // constructor

  // common method
  void prepare() noexcept
  {

  }

  void process(const AudioBlock<float>& buffer) noexcept
  {
    AudioBlock<float> block { buffer };
    ProcessContextReplacing<float> context { block };
    const AudioBlock<const float>& inputBlock = context.getInputBlock();
    AudioBlock<float>& outputBlock = context.getOutputBlock();
    const size_t numInChannels = inputBlock.getNumChannels();
    const size_t numOutChannels = outputBlock.getNumChannels();
    const size_t numSamples = outputBlock.getNumSamples();

    outputBlock.copyFrom(inputBlock);

    if (numOutChannels == 1)
      processMono(outputBlock.getChannelPointer(0), static_cast<int>(numSamples));
    else if (numOutChannels == 2)
      processStereo(outputBlock.getChannelPointer(0), outputBlock.getChannelPointer(1), intCast(numSamples));
    else
      processMono(outputBlock.getChannelPointer(0), static_cast<int>(numSamples));
  }

  void processStereo(float* const left, float* const right, const int numSamples) noexcept
  {

  }

  void processMono(float* const samples, const int numSamples) noexcept
  {

  }

private:
  // data member
};

} // namespace iNVOXRecords::processor