#pragma once

#include "iR_PresetSelectorComponent.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_PresetSelector class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetSelector : public Component
{
public:
  // constructor
  iR_PresetSelector(APVTS* apvts, iR_UserProperties* userProperties) :
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

  ~iR_PresetSelector()
  {
    setLookAndFeel(nullptr);
    list_button.getFileListComponent()->setLookAndFeel(nullptr);
    menu_button.getPopupMenu()->setLookAndFeel(nullptr);
  }

  // setter
  void setPosition(int x, int y, float scale)
  {
    x *= scale;
    y *= scale;
    const int w = 250 * scale;      // entire width
    const int lb_w = w * 0.8f;      // list_button width
    const int mb_w = w * 0.2f;      // menu_button width
    const int h = 30 * scale;       // entire height
    const int brow_h = 150 * scale; // file_browser height
    const int row_h = 25 * scale;   // row height

    setBounds(x, y, w, h);
    list_button.setBounds(0, 0, lb_w, h);
    list_button.getFileListComponent()->setBounds(x, y + h, w, brow_h);
    list_button.getFileListComponent()->setRowHeight(row_h);
    menu_button.setBounds(lb_w, 0, mb_w, h);
  }

  // getter
  iR_FileListComponentWrapper* getFileListComponent() { return list_button.getFileListComponent(); }

private:
  // lookandfeel
  iR_PresetSelectorLaF laf;

  APVTS* apvts;
  iR_UserProperties* user_properties;
  iR_DirectoryList list;
  iR_PresetListButton list_button = iR_PresetListButton(apvts, &list);
  iR_PresetMenuButton menu_button = iR_PresetMenuButton(user_properties, apvts, &list);
};

}
}