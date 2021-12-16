#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

inline float logMap0To1(float denormValue, float min, float max)
{
  return std::log(denormValue / min) / std::log(max / min);
}

namespace iNVOXRecords::gui {

class iR_GridComponent : public Component
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
  iR_GridComponent(const float& scale)
    : scale(scale)
  {
    setColour(backgroundColourId, Colours::grey);
    setColour(mainViewColourId, Colours::white);
    setColour(textColourId, Colours::white);
    setColour(lineColourId, Colours::black);
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
    const int x = initRect.getX() * scale;
    const int y = initRect.getY() * scale;
    const int w = initRect.getWidth() * scale;
    const int h = initRect.getHeight() * scale;
    const int mainViewX = w * mainViewReduceRatio[1];
    const int mainViewY = h * mainViewReduceRatio[0];
    const int mainViewW = w * (1.0f - (mainViewReduceRatio[1] + mainViewReduceRatio[3]));
    const int mainViewH = h * (1.0f - (mainViewReduceRatio[0] + mainViewReduceRatio[2]));

    setBounds(x, y, w, h);
    mainViewRect.setBounds(mainViewX, mainViewY, mainViewW, mainViewH);
  }

  // getter
  Rectangle<int> getMainViewBounds() noexcept
  {
    return mainViewRect;
  }

  // setter
  void setInitRectangle(const Rectangle<int>& newRect) noexcept
  {
    initRect = newRect;
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
  const float& scale;
  Rectangle<int> initRect;

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
    const float minX = rangeX.getStart();
    const float maxX = rangeX.getEnd();
    const int top = mainViewRect.getY();
    const int bottom = mainViewRect.getBottom();
    const Colour lineColour = findColour(lineColourId);
    const Font textFont { 10 * scale };
    const int textW = 30 * scale;
    const int textH = 10 * scale;

    g.setColour(lineColour);
    g.setFont(textFont);
    g.drawVerticalLine(mainViewRect.getX(), top, bottom);
    g.drawVerticalLine(mainViewRect.getRight(), top, bottom);

    for (auto&& s : scalesX) {
      #if JUCE_DEBUG
      if (useLogScaleOnXAxis) {
        jassert(rangeX.getStart() > 0);
      }
      #endif

      const float ratio = useLogScaleOnXAxis ? logMap0To1(s, minX, maxX) : (s - minX) / (maxX - minX);
      const int x = mainViewRect.getX() + mainViewRect.getWidth() * ratio;
      String text { s };

      if (valueToTextX) {
        text = valueToTextX(s);
      }

      g.drawFittedText(text, x, bottom, textW, textH, Justification::left, 1);
      g.drawVerticalLine(x, top, bottom);
    }
  }

  void drawGridY(Graphics& g)
  {
    const float minY = rangeY.getStart();
    const float maxY = rangeY.getEnd();
    const int left = mainViewRect.getX();
    const int right = mainViewRect.getRight();
    const Colour lineColour = findColour(lineColourId);
    const Font textFont { 10 * scale };
    const int textH = textFont.getHeight();
    const int textW = mainViewRect.getX() - getLocalBounds().getX();
    const int textRightOffset = 1.5f * scale;

    g.setColour(lineColour);
    g.setFont(textFont);
    g.drawHorizontalLine(mainViewRect.getY(), left, right);
    g.drawHorizontalLine(mainViewRect.getBottom(), left, right);

    for (auto&& s : scalesY) {
      #if JUCE_DEBUG
      if (useLogScaleOnYAxis) {
        jassert(rangeY.getStart() > 0);
      }
      #endif

      const float ratio = useLogScaleOnYAxis ? logMap0To1(s, minY, maxY) : (maxY - s) / (maxY - minY);
      const int y = mainViewRect.getY() + mainViewRect.getHeight() * ratio;
      const int textY = y - textH / 2;
      String text { s };

      if (valueToTextY) {
        text = valueToTextY(s);
      }

      g.drawFittedText(text, 0, textY, textW - textRightOffset, textH, Justification::right, 1);
      g.drawHorizontalLine(y, left, right);
    }
  }
};

} // namespace iNVOXRecords::gui