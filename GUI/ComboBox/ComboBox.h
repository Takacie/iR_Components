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

#include "ComboBoxLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// ComboBox class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
class ComboBox : public juce::ComboBox
{
public:
  // constructor
  ComboBox(APVTS& apvts, const String& parameterID, const StringArray& items, int default_index = 0) :
    apvts(&apvts),
    parameter_id(parameterID),
    attachment(*apvts.getParameter(parameterID), *this)
  {
    setLookAndFeel(lookandfeel.get());
    getRootMenu()->setLookAndFeel(lookandfeel.get());
    addItemList(items, 1);
    setSelectedItemIndex(default_index);

    String text = apvts.getParameter(parameterID)->getName(16);
    title_label.setFont(Font(16, Font::plain));
    title_label.setText(text, dontSendNotification);
    title_label.setJustificationType(Justification::centred);
    addAndMakeVisible(title_label);
  }

  // override
  void paint(Graphics& g) override
  {
    ComboBox::paint(g);
    setAlpha(isEnabled() ? 1.0f : 0.5f);
  }

  void showPopup() override
  {
    //ComboBox::showPopup(); <- discard isPopupActive()
    getRootMenu()->showMenuAsync(PopupMenu::Options()
      .withPreferredPopupDirection(PopupMenu::Options::PopupDirection::downwards)
      .withTargetComponent(this)
      .withMinimumWidth(getWidth())
      .withStandardItemHeight(getHeight() * 0.25f),
      [&](int choice) {
        hidePopup();
        if (choice != 0) setSelectedId(choice);
    });
  }

  // getter

  // setter
  void setPosition(int x, int y, float scale)
  {
    setBounds(x * scale, y * scale, 100 * scale, 110 * scale);
    title_label.setBounds(0, 87.5f * scale, 100 * scale, 20 * scale);
    title_label.setFont(Font(16 * scale, Font::plain));
  }

  // static
  static std::unique_ptr<ComboBoxLookAndFeel> lookandfeel;
  static void setComboBoxColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  Label title_label;
  ComboBoxParameterAttachment attachment;
};

} // namespace iNVOXRecords::gui