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

#include "juce_core/juce_core.h"
#include "juce_dsp/juce_dsp.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

using namespace juce;

namespace iNVOXRecords::processor::equalizer {
//----------------------------------------------------------------------------------------------------------------------
// constants
//----------------------------------------------------------------------------------------------------------------------
constexpr std::size_t MAX_FILTERS = 16; // except LPF and HPF
constexpr float HALF_SQRT2 = juce::MathConstants<float>::sqrt2 / 2;

// Shape
constexpr int INIT_SHAPE = 0;
const StringArray SYNC_SHAPE_CHOICES
{
  "Peak", "LowShelf",  "HighShelf", "BandPass", "Notch"
};

// Freqency limits
constexpr float MIN_FREQ = 20.0f;
constexpr float MAX_FREQ = 20'000.0f;

// Q limits
constexpr float MIN_Q = 0.1f;
constexpr float MAX_Q = 24.0f;

// Gain limits
constexpr float MIN_GAIN = -24.f;
constexpr float MAX_GAIN = 24.0f;

//----------------------------------------------------------------------------------------------------------------------
// enums
//----------------------------------------------------------------------------------------------------------------------
enum class FilterType {
  LowPass,
  HighPass,
  BandPass,
  Notch,
  LowShelf,
  HighShelf,
  Peak
};

//----------------------------------------------------------------------------------------------------------------------
// aliases
//----------------------------------------------------------------------------------------------------------------------
using APVTS = juce::AudioProcessorValueTreeState;
using FT = FilterType;
using Coeffs = dsp::IIR::Coefficients<float>;
using CoeffsPtr = std::unique_ptr<Coeffs>;
using Filter = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, Coeffs>;
using FilterPtr = std::unique_ptr<Filter>;
using FD = dsp::FilterDesign<float>;

//----------------------------------------------------------------------------------------------------------------------
// EqProcessor implementation
//----------------------------------------------------------------------------------------------------------------------
class EqProcessor
{
public:
  // constructor
  EqProcessor(APVTS& apvts) :
    apvts(apvts)
  {
    qs.fill(HALF_SQRT2);
  }

  // common method
  void prepareFilter(int index)
  {
    if (filters[index]) filters[index]->prepare(spec);
  }

  void prepare(const dsp::ProcessSpec& newSpec)
  {
    spec = newSpec;

    if (!lpf) generateFilter(FT::LowPass, 1000, -1);
    if (!hpf) generateFilter(FT::HighPass, 800, -1);

    if (lpf) {
      lpf->reset();
      lpf->prepare(newSpec);
    }

    if (hpf) {
      hpf->reset();
      hpf->prepare(newSpec);
    }

    for (auto&& filter : filters) {
      if (filter) {
        filter->reset();
        filter->prepare(newSpec);
      }
    }
  }

  void process(const juce::dsp::AudioBlock<float>& buffer) noexcept
  {
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);

    if (lpf) lpf->process(context);
    if (hpf) hpf->process(context);

    for (auto i = 0; i < MAX_FILTERS; ++i) {
      if (filters[i] && getParamEnable(i)) {
        filters[i]->process(context);
      }
    }
  }

  void removeFilter(int index)
  {
    filters[index].reset();
    coeffs[index].reset();

    setParamEnable(index, false);
  }

  void generateFilter(FT type, float freq, int index, float q = HALF_SQRT2, float gain = 1.0f)
  {
    const auto samplerate = spec.sampleRate;

    ReferenceCountedObjectPtr<Coeffs> coeff;
    switch (type)
    {
    case FT::LowPass:
      coeff = Coeffs::makeLowPass(samplerate, freq);
      lpf_coeffs = std::make_unique<Coeffs>(*coeff);
      lpf = std::make_unique<Filter>(*coeff);
      break;
    case FT::HighPass:
      coeff = Coeffs::makeHighPass(samplerate, freq);
      hpf_coeffs = std::make_unique<Coeffs>(*coeff);
      hpf = std::make_unique<Filter>(*coeff);
      break;
    case FT::Peak:
      coeff = Coeffs::makePeakFilter(samplerate, freq, q, gain);
      break;
    case FT::BandPass:
      break;
    case FT::LowShelf:
      break;
    case FT::HighShelf:
      break;
    case FT::Notch:
      break;
    default:
      break;
    }
    if (index < 0) return;
    // except lpf and hpf
    coeffs[index] = std::make_unique<Coeffs>(*coeff);
    filters[index] = std::make_unique<Filter>(*coeff);
    qs[index] = q;

    setParamEnable(index, true);
  }

  // getter
  APVTS& getAudioProcessorValueTreeState() { return apvts; }
  float getSamplerate() const { return spec.sampleRate; }
  FilterPtr& getLowPassFilter() { return lpf; }
  FilterPtr& getHighPassFilter() { return hpf; }
  FilterPtr& getFilter(int index) { return filters[index]; }
  CoeffsPtr& getLPFCoeffs() { return lpf_coeffs; }
  CoeffsPtr& getHPFCoeffs() { return hpf_coeffs; }
  CoeffsPtr& getCoeffs(int index) { return coeffs[index]; }
  float getQ(int index) const { return qs[index]; }

  // setter
  void updateFilterSettings(FT type, float freq, int index = -1, float q = HALF_SQRT2, float gain = 1.0f)
  {
    const auto samplerate = spec.sampleRate;

    ReferenceCountedObjectPtr<Coeffs> coeff;
    switch (type)
    {
    case FT::LowPass: {
      coeff = Coeffs::makeLowPass(samplerate, freq);
      lpf_coeffs = std::make_unique<Coeffs>(*coeff);
      *lpf->state = *coeff;
      break;
    }
    case FT::HighPass: {
      coeff = Coeffs::makeHighPass(samplerate, freq);
      hpf_coeffs = std::make_unique<Coeffs>(*coeff);
      *hpf->state = *coeff;
      break;
    }
    case FT::Peak: {
      coeff = Coeffs::makePeakFilter(samplerate, freq, q, gain);
      break;
    }
    case FT::BandPass: {
      break;
    }
    case FT::LowShelf: {
      break;
    }
    case FT::HighShelf: {
      break;
    }
    case FT::Notch: {
      break;
    }
    default:
      break;
    }

    if (index < 0) return;
    // except lpf and hpf
    coeffs[index] = std::make_unique<Coeffs>(*coeff);
    *filters[index]->state = *coeff;
    qs[index] = q;
  }

private:
  dsp::ProcessSpec spec{0, 441, 2};
  APVTS& apvts;
  FilterPtr lpf, hpf;
  std::array<FilterPtr, MAX_FILTERS> filters;
  CoeffsPtr lpf_coeffs, hpf_coeffs;
  std::array<CoeffsPtr, MAX_FILTERS> coeffs;
  std::array<float, MAX_FILTERS> qs; // array of filter Q

  // helper
  String getBandName(int index) const { return "Band " + String(index + 1); }

  RangedAudioParameter* getBandParameter(int index, const String& suffix) const
  {
    const String id = getBandName(index) + " " + suffix;
    return apvts.getParameter(id);
  }

  // param getter
  bool getParamEnable(int index) const
  { 
    return getBandParameter(index, "Enable")->getValue() > 0.5f;
  }

  float getParamFreq(int index) const
  {
    return getBandParameter(index, "Freq")->getValue();
  }

  // param setter
  void setParamEnable(int index, bool newValue)
  {
    getBandParameter(index, "Enable")->setValueNotifyingHost(newValue);
  }
};

} // namespace iNVOXRecords::processor::equalizer