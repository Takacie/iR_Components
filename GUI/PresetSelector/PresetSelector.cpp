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
  user_properties(userProperties)
{
  addAndMakeVisible(list_button);
  addAndMakeVisible(menu_button);

  setLookAndFeel(&laf);
  list_button.getFileListComponent()->setLookAndFeel(&laf);
  menu_button.getPopupMenu()->setLookAndFeel(&laf);
  list_button.getFileListComponent()->setVisible(false);
}

PresetSelector::~PresetSelector()
{
  setLookAndFeel(nullptr);
  list_button.getFileListComponent()->setLookAndFeel(nullptr);
  menu_button.getPopupMenu()->setLookAndFeel(nullptr);
}

// setter
void PresetSelector::setPosition(int x, int y, float scale)
{
  x *= scale;
  y *= scale;
  const auto w = getWidth();       // entire width
  const auto lb_w = w * 0.8f;      // list_button width
  const auto mb_w = w * 0.2f;      // menu_button width
  const auto h = 30 * scale;       // entire height
  const auto brow_h = 150 * scale; // file_browser height
  const auto row_h = 25 * scale;   // row height

  setBounds(x, y, w, h);
  list_button.setBounds(0, 0, lb_w, h);
  list_button.getFileListComponent()->setBounds(x, y + h, w, brow_h);
  list_button.getFileListComponent()->setRowHeight(row_h);
  menu_button.setBounds(lb_w, 0, mb_w, h);
}

} // namespace iNVOXRecords::gui