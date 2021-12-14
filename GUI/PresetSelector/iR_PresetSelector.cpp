#include "iR_PresetSelector.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_PresetSelector implementation
//----------------------------------------------------------------------------------------------------------------------
iR_PresetSelector::iR_PresetSelector(APVTS* apvts, iR_UserProperties* userProperties) :
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

iR_PresetSelector::~iR_PresetSelector()
{
  setLookAndFeel(nullptr);
  list_button.getFileListComponent()->setLookAndFeel(nullptr);
  menu_button.getPopupMenu()->setLookAndFeel(nullptr);
}

// setter
void iR_PresetSelector::setPosition(int x, int y, float scale)
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

}
}