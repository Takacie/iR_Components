#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_EQHandle class
//----------------------------------------------------------------------------------------------------------------------
class iR_EQHandle : public Component
{
public:
  // constructor
  iR_EQHandle();

  // override
  void paint(Graphics& g) override
  {
    g.setColour(Colours::white);
    g.fillAll();
  }

  // common method

  // getter

  // setter

private:
  // member
};

} // namespace GUI
} // namespace iNVOXRecords