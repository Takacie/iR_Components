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

#include "../TextEditor/TextEditor.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_Label class
//----------------------------------------------------------------------------------------------------------------------
class Label : public juce::Label
{
public:
  // constructor
  Label(const float& scale) : scale(scale)
  {
    setJustificationType(Justification::centred);
  }

  // override
  void paint(Graphics& g) override
  {
    // draw background
    g.fillAll(backgroundColour);

    // draw frame
    const int frameLineWidth = scale;
    g.setColour(frameColour);
    g.drawRect(getLocalBounds(), frameLineWidth);

    // draw text
    if (isBeingEdited()) return;
    const int fontHeight = getBounds().getHeight() * fontHeightScale;
    const auto textRect = getLocalBounds().reduced(paddingLeftRight * scale, 0);
    g.setFont(fontHeight);
    g.setColour(textColour);
    g.drawText(getText(), textRect, getJustificationType());
  }

  void parentSizeChanged() override
  {
    const int x = initBounds.getX() * scale;
    const int y = initBounds.getY() * scale;
    const int w = initBounds.getWidth() * scale;
    const int h = initBounds.getHeight() * scale;
    setBounds(x, y, w, h);
  }

  TextEditor* createEditorComponent() override
  {
    auto textEditor = new TextEditor(scale);
    textEditor->setJustification(getJustificationType());
    textEditor->setFont(getBounds().getHeight() * fontHeightScale);

    return textEditor;
  }

  // getter

  // setter
  void setBackgroundColour(const Colour& colour) noexcept { backgroundColour = colour; }
  void setFrameColour(const Colour& colour) noexcept { frameColour = colour; }
  void setTextColour(const Colour& colour) noexcept { textColour = colour; }

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
  Colour frameColour { Colours::transparentBlack };
  Colour textColour { Colours::white };
};

} // namespace iNVOXRecords::gui