#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_FileBrowserLaF class
//----------------------------------------------------------------------------------------------------------------------
class iR_FileBrowserLaF : public LookAndFeel_V4
{
public:
  void drawFileBrowserRow(Graphics& g, int width, int height, const File& file, const String& filename, Image* icon,
    const String& fileSizeDescription, const String& fileTimeDescription, bool isDirectory,
    bool isItemSelected, int itemIndex, DirectoryContentsDisplayComponent& dcc)
  {
    if (isItemSelected) {
      g.fillAll(Colour(10, 10, 10));
    }

    const int scale_basis = height;
    const int x = scale_basis * 0.25f;
    g.setColour(Colours::white);
    g.setFont(scale_basis * 0.7f);

    const String text = filename.replace(".irps", "");
    g.drawFittedText(text, x, 0, width - x, height, Justification::centredLeft, 1);
  }
};

}
}