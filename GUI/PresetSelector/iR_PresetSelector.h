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
  iR_PresetSelector(APVTS* apvts, iR_UserProperties* userProperties);

  ~iR_PresetSelector();

  // setter
  void setPosition(int x, int y, float scale);
  void setWidth(int newWidth) { setBounds(0, 0, newWidth, 0); }

  // getter
  iR_FileListComponent* getFileListComponent() { return list_button.getFileListComponent(); }

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