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
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// GridComponent class
//----------------------------------------------------------------------------------------------------------------------
class GridComponent : public juce::Component, public ResizeInterface
{
public:
  // enum
  enum class Axis
  {
    axisX,
    axisY
  };

  enum ColourIds
  {
    backgroundColourId,
    mainViewColourId,
    textColourId,
    lineColourId
  };

  // constructor
  GridComponent(const float& scale)
    : ResizeInterface(scale)
  {
    setColour(backgroundColourId, Colour(10, 10, 15));
    setColour(mainViewColourId, Colour(20, 20, 25));
    setColour(textColourId, Colour(180, 180, 185));
    setColour(lineColourId, Colour(60, 60, 65));
  }

  // override
  void paint(Graphics& g) override
  {
    const Colour backgroundColour = findColour(backgroundColourId);
    const Colour mainViewColour = findColour(mainViewColourId); 

    g.fillAll(backgroundColour);

    g.setColour(mainViewColour);
    g.fillRect(mainViewRect);

    drawGridX(g);
    drawGridY(g);
  }

  void parentHierarchyChanged() override
  {
    parentSizeChanged();
  }

  void parentSizeChanged() override
  {
    const int x = getScaledX();
    const int y = getScaledY();
    const int w = getScaledWidth();
    const int h = getScaledHeight();
    const int mainViewX = w * mainViewReduceRatio[1];                                      // width * leftReduceRatio
    const int mainViewY = h * mainViewReduceRatio[0];                                      // height * topReduceRatio
    const int mainViewW = w * (1.0f - (mainViewReduceRatio[1] + mainViewReduceRatio[3]));  // width * (1.0 - (leftReduceRatio + rightReduceRatio))
    const int mainViewH = h * (1.0f - (mainViewReduceRatio[0] + mainViewReduceRatio[2]));  // height * (1.0 - (topReduceRatio + bottomReduceRatio))

    setBounds(x, y, w, h);
    mainViewRect.setBounds(mainViewX, mainViewY, mainViewW, mainViewH);
  }

  // getter
  Rectangle<int> getMainViewBounds() noexcept
  {
    return mainViewRect;
  }

  Rectangle<int> getInitMainViewBounds() noexcept
  {
    const int mainViewX = getInitWidth() * mainViewReduceRatio[1];
    const int mainViewY = getInitHeight() * mainViewReduceRatio[0];
    const int mainViewW = getInitWidth() * (1.0f - (mainViewReduceRatio[1] + mainViewReduceRatio[3]));
    const int mainViewH = getInitHeight() * (1.0f - (mainViewReduceRatio[0] + mainViewReduceRatio[2]));
    return Rectangle<int>(mainViewX, mainViewY, mainViewW, mainViewH);
  }

  void setRange(const Axis axis, const Range<float>& newRange) noexcept
  {
    
    (axis == Axis::axisX ? rangeX : rangeY) = newRange;
  }

  void setScales(const Axis axis, std::vector<float>& newScales) noexcept
  {
    (axis == Axis::axisX ? scalesX : scalesY) = std::move(newScales);
  }

  void setUseLogScale(const Axis axis, bool useLogScale) noexcept
  {
    (axis == Axis::axisX ? useLogScaleOnXAxis : useLogScaleOnYAxis) = useLogScale;
  }

  void setMainViewReduceRatio(float top, float left, float bottom, float right) noexcept
  {
    mainViewReduceRatio = { top, left, bottom, right };
  }

  void setValueToTextFunction(const Axis axis, std::function<String(float)> func) noexcept
  {
    (axis == Axis::axisX ? valueToTextX : valueToTextY) = std::move(func);
  }

private:
  Rectangle<int> mainViewRect;
  std::array<float, 4> mainViewReduceRatio { 0.03f, 0.075f, 0.075f, 0.03f }; // top, left, bottom, right

  Range<float> rangeX;
  Range<float> rangeY;
  std::vector<float> scalesX;
  std::vector<float> scalesY;

  std::function<String(float)> valueToTextX;
  std::function<String(float)> valueToTextY;

  bool useLogScaleOnXAxis = false;
  bool useLogScaleOnYAxis = false;

  // helper
  void drawGridX(Graphics& g)
  {
    const float minValueX = rangeX.getStart();
    const float maxValueX = rangeX.getEnd();
    const int top = mainViewRect.getY();
    const int bottom = mainViewRect.getBottom();
    const Colour lineColour = findColour(lineColourId);
    const Colour textColour = findColour(textColourId);
    const Font textFont { 10 * static_cast<float>(getScale()) };
    const int textW = 30 * getScale();
    const int textH = 10 * getScale();
    const int textTopOffset = 2.0f * getScale();

    g.setFont(textFont);

    for (auto&& s : scalesX) {
      #if JUCE_DEBUG
      if (useLogScaleOnXAxis) {
        jassert(rangeX.getStart() > 0);
      }
      #endif

      const float ratio = useLogScaleOnXAxis ?
        mapFromLog10(s, minValueX, maxValueX) :
        (s - minValueX) / (maxValueX - minValueX);
      const int x = mainViewRect.getX() + mainViewRect.getWidth() * ratio;
      String text { s };

      if (valueToTextX) {
        text = valueToTextX(s);
      }

      g.setColour(textColour);
      g.drawFittedText(text, x, bottom + textTopOffset, textW, textH, Justification::left, 1);
      g.setColour(lineColour);
      g.drawVerticalLine(x, top, bottom);
    }

    g.drawVerticalLine(mainViewRect.getX(), top, bottom);
    g.drawVerticalLine(mainViewRect.getRight(), top, bottom);
  }

  void drawGridY(Graphics& g)
  {
    const float minValueY = rangeY.getStart();
    const float maxValueY = rangeY.getEnd();
    const int left = mainViewRect.getX();
    const int right = mainViewRect.getRight();
    const Colour lineColour = findColour(lineColourId);
    const Colour textColour = findColour(textColourId);
    const Font textFont { 10 * static_cast<float>(getScale()) };
    const int textH = textFont.getHeight();
    const int textW = mainViewRect.getX() - getLocalBounds().getX();
    const int textRightOffset = 3.0f * getScale();

    g.setFont(textFont);

    for (auto&& s : scalesY) {
      #if JUCE_DEBUG
      if (useLogScaleOnYAxis) {
        jassert(rangeY.getStart() > 0);
      }
      #endif

      const float ratio = useLogScaleOnYAxis ? 
        mapFromLog10(s, minValueY, maxValueY) :
        (maxValueY - s) / (maxValueY - minValueY);
      const int y = mainViewRect.getY() + mainViewRect.getHeight() * ratio;
      const int textY = y - textH / 2;
      String text { s };

      if (valueToTextY) {
        text = valueToTextY(s);
      }

      g.setColour(textColour);
      g.drawFittedText(text, 0, textY, textW - textRightOffset, textH, Justification::right, 1);
      g.setColour(lineColour);
      g.drawHorizontalLine(y, left, right);
    }

    g.drawHorizontalLine(mainViewRect.getY(), left, right);
    g.drawHorizontalLine(mainViewRect.getBottom(), left, right);
  }
};

} // namespace iNVOXRecords::gui
