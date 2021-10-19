#pragma once

#include "iR_PresetSelectorLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_PresetSelector class
//----------------------------------------------------------------------------------------------------------------------
using APVTS = AudioProcessorValueTreeState;
class iR_PresetSelector
{
public:
  // constructor
  iR_PresetSelector();

  // common method
  void addAndMakeVisibleMyself(AudioProcessorEditor& editor);

  // setter
  void setPosition(int x, int y, float size_ratio);

private:

};

}
}