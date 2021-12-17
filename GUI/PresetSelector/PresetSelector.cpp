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

#include "PresetSelector.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// PresetSelector implementation
//----------------------------------------------------------------------------------------------------------------------
PresetSelector::PresetSelector(APVTS* apvts, UserProperties* userProperties) :
  apvts(apvts),
  userProperties(userProperties)
{
  addAndMakeVisible(listButton);
  addAndMakeVisible(menuButton);

  setLookAndFeel(&laf);
  listButton.getFileListComponent()->setLookAndFeel(&laf);
  menuButton.getPopupMenu()->setLookAndFeel(&laf);
  listButton.getFileListComponent()->setVisible(false);
}

PresetSelector::~PresetSelector()
{
  setLookAndFeel(nullptr);
  listButton.getFileListComponent()->setLookAndFeel(nullptr);
  menuButton.getPopupMenu()->setLookAndFeel(nullptr);
}

// setter
void PresetSelector::setPosition(int x, int y, float scale)
{
  x *= scale;
  y *= scale;
  const auto w = getWidth();       // entire width
  const auto lbW = w * 0.8f;      // listButton width
  const auto mbW = w * 0.2f;      // menuButton width
  const auto h = 30 * scale;       // entire height
  const auto browH = 150 * scale; // file_browser height
  const auto rowH = 25 * scale;   // row height

  setBounds(x, y, w, h);
  listButton.setBounds(0, 0, lbW, h);
  listButton.getFileListComponent()->setBounds(x, y + h, w, browH);
  listButton.getFileListComponent()->setRowHeight(rowH);
  menuButton.setBounds(lbW, 0, mbW, h);
}

} // namespace iNVOXRecords::gui