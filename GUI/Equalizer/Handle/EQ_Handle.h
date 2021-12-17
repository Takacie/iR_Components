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

#include "../../../Processor/Equalizer/EQ_Processor.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

using namespace juce;

namespace iNVOXRecords::gui::equalizer {
using namespace processor::equalizer;

class EqGraphicController;
//----------------------------------------------------------------------------------------------------------------------
// constants
//----------------------------------------------------------------------------------------------------------------------
constexpr float WIDTH = 10.0f;

//----------------------------------------------------------------------------------------------------------------------
// aliases
//----------------------------------------------------------------------------------------------------------------------
using CBC = ComponentBoundsConstrainer;
using AttachmentPtr = std::unique_ptr<ParameterAttachment>;

//----------------------------------------------------------------------------------------------------------------------
// EqHandle class
//----------------------------------------------------------------------------------------------------------------------
class EqHandle : public juce::Component
{
public:
  // constructor
  EqHandle(EqGraphicController& parent, EqProcessor& processor, 
    const Point<int>& initialPos, int index, FT type = FT::Peak);

  // override
  void paint(Graphics& g) override;

  void parentSizeChanged() override;

  void mouseDoubleClick(const MouseEvent& event) override;

  void mouseDrag(const MouseEvent& event) override;

  void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

  void mouseDown(const MouseEvent& event) override
  {
    dragger.startDraggingComponent(this, event);

    if (freqAttachment) freqAttachment->beginGesture();
  }

  void mouseUp(const MouseEvent& event) override
  {
    updateRelativePos();

    if (freqAttachment) freqAttachment->endGesture();
  }

  // common method
  void resetFilterSettings(const float q = HALF_SQRT2);

  float calcGainFactor();
  float calcFrequency();

  // getter

  // setter
  void setFilterType(const FT newType) { type = newType; }

  void setHandleColour(const Colour& newColour) { colour = newColour; }


private:
  // member
  EqGraphicController& parent;
  EqProcessor& processor;
  const int handleIndex;
  FT type;
  std::array<float, 2> relativePos{ -1, -1 };
  Colour colour;
  CBC constrainer;
  ComponentDragger dragger;

  AttachmentPtr freqAttachment;
  AttachmentPtr qAttachment;

  // helper
  bool isLPForHPF() { return (type == FT::HighPass || type == FT::LowPass); }

  bool isValidRelativePos() { return (relativePos[0] != -1 && relativePos[1] != -1); }

  void updateRelativePos();

  void initFreqAttachment()
  {
    auto apvts = &processor.getAudioProcessorValueTreeState();
    const String name = "Band " + String(handleIndex + 1) + " Freq";
    auto& param = *apvts->getParameter(name);

    freqAttachment = std::make_unique<ParameterAttachment>(
      param,
      [this](float newFreq) { setFreq(newFreq); },
      nullptr
    );

    freqAttachment->sendInitialUpdate();
  }

  void setFreq(float newFreq)
  {
    if (!isLPForHPF()) {
      auto apvts = &processor.getAudioProcessorValueTreeState();
      const String name = "Band " + String(handleIndex + 1) + " Freq";
      auto param = apvts->getParameter(name);
      const float param_value = *apvts->getRawParameterValue(name);

      updateRelativePos();
      relativePos[0] = jmap((MAX_FREQ - param_value) / (MAX_FREQ - MIN_FREQ), MIN_FREQ, MAX_FREQ);
      DBG(mapFromLog10(param_value, MIN_FREQ, MAX_FREQ));
      setCentreRelative(0, relativePos[1]);
      updateRelativePos();
    }
  }
};

} // namespace iNVOXRecords::gui::equalizer