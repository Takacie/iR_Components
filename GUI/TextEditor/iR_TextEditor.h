#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_TextEditor class
//----------------------------------------------------------------------------------------------------------------------
class iR_TextEditor : public TextEditor
{
public:
  // constructor
  iR_TextEditor(const float& scale) : scale(scale)
  {
  }

  // override
  void paint(Graphics& g) override
  {
    // draw background
    g.fillAll(backgroundColour);
  }

  void paintOverChildren(Graphics& g) override
  {
    // draw frame
    const int frameLineWidth = scale;

    if (hasKeyboardFocus(true) && !isReadOnly()) {
      g.setColour(focusedFrameColour);
      g.drawRect(getLocalBounds(), frameLineWidth * 2);
    }
    else {
      g.setColour(frameColour);
      g.drawRect(getLocalBounds(), frameLineWidth);
    }
  }

  void parentSizeChanged() override
  {
    const int x = initBounds.getX() * scale;
    const int y = initBounds.getY() * scale;
    const int w = initBounds.getWidth() * scale;
    const int h = initBounds.getHeight() * scale;

    setBounds(x, y, w, h);
    applyFontToAllText(h * fontHeightScale);
  }

  // getter

  // setter
  void setBackgroundColour(const Colour& colour) noexcept { backgroundColour = colour; }
  void setFrameColour(const Colour& colour) noexcept { frameColour = colour; }
  void setFocusedFrameColour(const Colour& colour) noexcept { focusedFrameColour = colour; }

  void setTextColour(const Colour& colour) noexcept
  {
    setColour(textColourId, colour);
  }

  void setHighlightedColour(const Colour& colour) noexcept
  {
    setColour(highlightColourId, colour);
  }

  void setCursorBarColour(const Colour& colour) noexcept
  {
    auto caretComponent = static_cast<CaretComponent*>(getChildComponent(0));
    caretComponent->setColour(caretComponent->caretColourId, colour);
  }

  void setInitBounds(const Rectangle<int>& rect) noexcept
  {
    setBounds(rect);
    initBounds = rect;
    parentSizeChanged();
  }

  void setPaddingLeftRight(int padding) noexcept { paddingLeftRight = padding; }
  void setFontHeightScale(float scale) noexcept { fontHeightScale = scale; }

private:
  const float& scale;
  Rectangle<int> initBounds;

  int paddingLeftRight { 0 };
  float fontHeightScale { 0.8f };

  Colour backgroundColour { 50, 50, 50 };
  Colour frameColour { Colours::white };
  Colour focusedFrameColour { Colours::white };
  Colour textColour { Colours::white };
};

} // namespace iNVOXRecords::gui