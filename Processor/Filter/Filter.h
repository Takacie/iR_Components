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
#include "juce_dsp/juce_dsp.h"

using namespace juce;
using namespace dsp;
using namespace iNVOXRecords::utility;

namespace iNVOXRecords::processor {
// constant
constexpr float INITIAL_Q = 1.0f / MathConstants<double>::sqrt2;
constexpr int MAX_ORDER = 8;

// aliase
using Coeffs = dsp::IIR::Coefficients<float>;
using Filter = juce::dsp::IIR::Filter<float>;

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
  IIRFilter(FilterType initType, int initOrder) :
    type(initType),
    order(initOrder)
  {
    filters2ndOrder.resize(order / 2);
  }

  // common method
  void prepare(const ProcessSpec& spec) noexcept
  {
    processSpec = spec;
    filter1stOrder.prepare(spec);

    for (auto&& filter2ndOrder : filters2ndOrder)
      filter2ndOrder.prepare(spec);
  }

  void calcCoeffs()
  {
    using CoeffsPtr = ReferenceCountedObjectPtr<Coeffs>;

    const float samplerate = processSpec.sampleRate;

    switch (type) {
      case FilterType::LowCut:
        if (order % 2 == 1) {
          coeffsFor1stOrder = *Coeffs::makeFirstOrderHighPass(samplerate, freq);
          *filter1stOrder.coefficients = coeffsFor1stOrder;
        }
        coeffsFor2ndOrder = *Coeffs::makeHighPass(samplerate, freq, q);
        break;
      case FilterType::HighCut:
        if (order % 2 == 1) {
          coeffsFor1stOrder = *Coeffs::makeFirstOrderLowPass(samplerate, freq);
          *filter1stOrder.coefficients = coeffsFor1stOrder;
        }
        coeffsFor2ndOrder = *Coeffs::makeLowPass(samplerate, freq, q);
        break;
      case FilterType::Peak:
        coeffsFor2ndOrder = *Coeffs::makePeakFilter(samplerate, freq, q, gain);
        break;
      case FilterType::LowShelf:
        coeffsFor2ndOrder = *Coeffs::makeLowShelf(samplerate, freq, q, gain);
        break;
      case FilterType::HighShelf:
        coeffsFor2ndOrder = *Coeffs::makeHighShelf(samplerate, freq, q, gain);
        break;
      case FilterType::BandPass:
        coeffsFor2ndOrder = *Coeffs::makeBandPass(samplerate, freq, q);
        break;
      case FilterType::Notch:
        coeffsFor2ndOrder = *Coeffs::makeNotch(samplerate, freq, q);
        break;
      default:
        break;
    }

    for (auto&& filter2ndOrder : filters2ndOrder)
      *filter2ndOrder.coefficients = coeffsFor2ndOrder;
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

    calcCoeffs();

    if (numOutChannels == 1)
      processMono(context);
    else if (numOutChannels == 2)
      processStereo(context);
  }

  void processStereo(ProcessContextReplacing<float> context) noexcept
  {

  }

  void processMono(ProcessContextReplacing<float> context) noexcept
  {
    if (order % 2 == 1)
      filter1stOrder.process(context);

    for (auto&& filter2ndOrder : filters2ndOrder)
      filter2ndOrder.process(context);
  }

  // setter
  void setOrder(int newOrder)
  {
    order = newOrder;
    filters2ndOrder.resize(newOrder);

    for (auto&& filter2ndOrder : filters2ndOrder)
      filter2ndOrder.prepare(processSpec);
  }

private:
  // data member
  FilterType type;
  int order;
  Coeffs coeffsFor1stOrder;
  Coeffs coeffsFor2ndOrder;
  Filter filter1stOrder;
  std::vector<Filter> filters2ndOrder;
  ProcessSpec processSpec { 0, 0, 0 };

  float freq = 500;
  float q = INITIAL_Q;
  float gain = 0.0f;

  // helper
};

} // namespace iNVOXRecords::processor