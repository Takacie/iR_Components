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

#include "PresetSelectorComponent.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// PresetSelector class
//----------------------------------------------------------------------------------------------------------------------
class PresetSelector : public juce::Component
{
public:
  // constructor
  PresetSelector(APVTS* apvts, UserProperties* userProperties);

  ~PresetSelector();

  // setter
  void setPosition(int x, int y, float scale);
  void setWidth(int newWidth) { setBounds(0, 0, newWidth, 0); }

  // getter
  FileListComponent* getFileListComponent() { return list_button.getFileListComponent(); }

private:
  // lookandfeel
  PresetSelectorLaF laf;

  APVTS* apvts;
  UserProperties* user_properties;
  DirectoryList list;
  PresetListButton list_button { apvts, &list };
  PresetMenuButton menu_button { user_properties, apvts, &list };
};

} // namespace iNVOXRecords::gui