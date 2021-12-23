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

#include "../ResizeInterface/ResizeInterface.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "../../Utility/Helper/Helper.h"

using namespace juce;
using namespace iNVOXRecords::utility;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// XYController class
//----------------------------------------------------------------------------------------------------------------------
class XYController : public juce::Component, public ResizeInterface
{
public:
  // constructor
  XYController(const float& scale, const APVTS& apvts, const StringRef& paramIdX, const StringRef& paramIdY) :
    ResizeInterface(scale),
    apvts(apvts),
    paramX(*apvts.getParameter(paramIdX)),
    paramY(*apvts.getParameter(paramIdY))
  {}

  // override
  void paint(Graphics& g) override
  {
    g.fillAll(Colours::white);
  }

  void parentHierarchyChanged() override
  {
    attachmentX.sendInitialUpdate();
    attachmentY.sendInitialUpdate();
  }

  void mouseDown(const MouseEvent& event) override
  {
    isEditing = true;
    attachmentX.beginGesture();
    attachmentY.beginGesture();

    dragger.startDraggingComponent(this, event);
  }

  void mouseDrag(const MouseEvent& event) override
  {
    const int halfW = getWidth() / 2;
    const int halfH = getHeight() / 2;
    constrainer.setMinimumOnscreenAmounts(halfH, halfW, halfH, halfW);
    dragger.dragComponent(this, event, &constrainer);

    const float paramValueX = calcValueParamX();
    const float paramValueY = calcValueParamY();

    attachmentX.setValueAsPartOfGesture(paramValueX);
    attachmentY.setValueAsPartOfGesture(paramValueY);

    getParentComponent()->repaint();
  }

  void mouseUp(const MouseEvent& event) override
  {
    isEditing = false;
    attachmentX.endGesture();
    attachmentY.endGesture();
  }

  void parentSizeChanged() override
  {
    attachmentX.sendInitialUpdate();
    attachmentY.sendInitialUpdate();
  }

  // getter
  const APVTS& getAudioProcessorValueTreeState() const noexcept { return apvts; }

  NormalisableRange<float> getRangeX() const noexcept { return rangeX; }
  NormalisableRange<float> getRangeY() const noexcept { return rangeY; }

  float getValueX() const noexcept { return valueX; }
  float getValueY() const noexcept { return valueY; }

protected:
  virtual float calcValueParamX() const noexcept
  {
    const float posX = getBounds().getCentre().getX();
    const float ratioX = posX / getParentWidth();
    const float denormX = rangeX.convertFrom0to1(clamp0To1(ratioX));

    return denormX;
  }

  virtual float calcValueParamY() const noexcept
  {
    const float posY = getBounds().getCentre().getY();
    const float ratioY = 1.0f - posY / getParentHeight();
    const float denormY = rangeY.convertFrom0to1(clamp0To1(ratioY));

    return denormY;
  }

  virtual void setXFromValue(float denormalizedValue) noexcept
  {
    const float normX = rangeX.convertTo0to1(denormalizedValue);
    const int halfW = getWidth() / 2;
    const int x = getParentWidth() * normX - halfW;

    #if JUCE_DEBUG
    jassert(x >= -halfW);
    jassert(x <= getParentWidth() + halfW);
    DBG("X: " + String(x));
    #endif

    setBounds(x, getY(), getScaledWidth(), getScaledHeight());
  }

  virtual void setYFromValue(float denormalizedValue) noexcept
  {
    const float normY = rangeY.convertTo0to1(denormalizedValue);
    const int halfH = getHeight() / 2;
    const int y = getParentHeight() * (1.0f - normY) - halfH;

    #if JUCE_DEBUG
    jassert(y >= -halfH);
    jassert(y <= getParentHeight() + halfH);
    DBG("Y: " + String(y));
    #endif

    setBounds(getX(), y, getScaledWidth(), getScaledHeight());
  }

private:
  // member
  const APVTS& apvts;
  RangedAudioParameter& paramX;
  RangedAudioParameter& paramY;
  NormalisableRange<float> rangeX { paramX.getNormalisableRange() };
  NormalisableRange<float> rangeY { paramY.getNormalisableRange() };
  float valueX { 0.0f };
  float valueY { 0.0f };

  ParameterAttachment attachmentX {
    paramX,
    [&](float denormalizedValue)
    {
      valueX = denormalizedValue;
      if (!isEditing) {
        setXFromValue(denormalizedValue);
        getParentComponent()->repaint();
      }
    },
    nullptr
  };

  ParameterAttachment attachmentY {
    paramY,
    [&](float denormalizedValue)
    {
      valueY = denormalizedValue;
      if (!isEditing) {
        setYFromValue(denormalizedValue);
        getParentComponent()->repaint();
      }
    },
    nullptr
  };

  ComponentDragger dragger;
  CBC constrainer;
  bool isEditing = false;
};

} // namespace iNVOXRecords::gui