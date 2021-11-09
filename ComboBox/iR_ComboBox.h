#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// Button class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
class iR_ComboBox : public ComboBox
{
public:
  // constructor
  iR_ComboBox(APVTS& apvts, const String& parameterID, const StringArray& items) :
    apvts(&apvts),
    parameter_id(parameterID),
    attachment(*apvts.getParameter(parameterID), *this)
  {
    addItemList(items, 1);
  }
  
  // override
  void paint(Graphics& g) override
  {
    ComboBox::paint(g);
  }

  // getter

  // setter
  void setPosition(int x, int y, float size_ratio)
  {

  }

  // static
  static std::unique_ptr<iR_ComboBoxLookAndFeel> lookandfeel;
  static void setComboBoxColor(const Colour& colour) { if (lookandfeel) lookandfeel->setMainColour(colour); }

private:
  APVTS* apvts;
  String parameter_id;
  ComboBoxParameterAttachment attachment;
};

} // namespace GUI
} // namespace iNVOXRecords