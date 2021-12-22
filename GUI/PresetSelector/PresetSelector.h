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
#include "PresetSelectorComponent.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// PresetSelector class
//----------------------------------------------------------------------------------------------------------------------
class PresetSelector : public juce::Component, public ResizeInterface
{
public:
  // constructor
  PresetSelector(const float& scale, APVTS& apvts, UserProperties* userProperties);

  ~PresetSelector();

  // override
  void parentSizeChanged() override
  {
    const float s = getScale();
    const int x = getScaledX();
    const int y = getScaledY();
    const int w = getScaledWidth();       // entire width
    const int h = getScaledHeight();       // entire height
    const int lbW = w * 0.8f;      // listButton width
    const int mbW = w * 0.2f;      // menuButton width
    //const auto h = 30 * scale;       // entire height
    const int browH = 150 * s; // file_browser height
    const int rowH = 25 * s;   // row height

    setBounds(x, y, w, h);
    listButton.setBounds(0, 0, lbW, h);
    listButton.getFileListComponent()->setBounds(x, y + h, w, browH);
    listButton.getFileListComponent()->setRowHeight(rowH);
    menuButton.setBounds(lbW, 0, mbW, h);
  }

  void parentHierarchyChanged() override { parentSizeChanged(); }

  // setter
  void setWidth(int newWidth) { setBounds(0, 0, newWidth, 0); }

  // getter
  FileListComponent* getFileListComponent() { return listButton.getFileListComponent(); }

private:
  // lookandfeel
  PresetSelectorLaF laf;

  APVTS& apvts;
  UserProperties* userProperties;
  DirectoryList list;
  PresetListButton listButton { apvts, &list };
  PresetMenuButton menuButton { userProperties, apvts, &list };
};

} // namespace iNVOXRecords::gui