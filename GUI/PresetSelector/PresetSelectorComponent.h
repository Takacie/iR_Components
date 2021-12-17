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

#include "PresetSelectorLookAndFeel.h"
#include "../../Utility/UserProperties/UserProperties.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;
using namespace iNVOXRecords::utility;

using APVTS = AudioProcessorValueTreeState;

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// iR_DirectoryList class
//----------------------------------------------------------------------------------------------------------------------
class DirectoryList
{
public:
  // constructor
  DirectoryList();

  // getter
  DirectoryContentsList* getDirectoryContentsList() { return &list; }

private:
  TimeSliceThread thread = TimeSliceThread("preset_read_thread");
  WildcardFileFilter filter = WildcardFileFilter("*.irps", "", "iNVOXRecords preset file.");
  DirectoryContentsList list = DirectoryContentsList(&filter, thread);
};

//----------------------------------------------------------------------------------------------------------------------
// FileListComponentWrapper class
//----------------------------------------------------------------------------------------------------------------------
class FileListComponent : public juce::FileListComponent, public juce::FileBrowserListener
{
public:
  // constructor
  FileListComponent(DirectoryContentsList& directoryContentList, APVTS* apvts);

  // override
  void selectionChanged() override {}
  void fileClicked(const File& file, const MouseEvent& e) override {}
  void browserRootChanged(const File& newRoot) override {}
  void fileDoubleClicked(const File& file) override;
  void inputAttemptWhenModal() override;

  // getter
  String getPresetName() const { return preset_name; }

private:
  String preset_name = "(default)";
  APVTS* apvts;
};

//----------------------------------------------------------------------------------------------------------------------
// PresetMenuButton class
//----------------------------------------------------------------------------------------------------------------------
class PresetMenuButton : public juce::Button
{
public:
  // constructor
  PresetMenuButton(UserProperties* user_properties, APVTS* apvts, DirectoryList* dirList);

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void mouseDown(const MouseEvent& event) override;

  // getter
  PopupMenu* getPopupMenu() { return &popup_menu; }

private:
  PopupMenu popup_menu;
  UserProperties* user_properties;
  APVTS* apvts;
  DirectoryList* dirList;
  std::unique_ptr<FileChooser> file_chooser;
};

//----------------------------------------------------------------------------------------------------------------------
// PresetListButton class
//----------------------------------------------------------------------------------------------------------------------
class PresetListButton : public juce::Button
{
public:
  // constructor
  PresetListButton(APVTS* apvts, DirectoryList* dir_list);

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void mouseDown(const MouseEvent& event) override;

  // getter
  FileListComponent* getFileListComponent() { return file_list_component.get(); }

  // setter
  void setPosition(Rectangle<int>& rect) { setBounds(rect); }

private:
  APVTS* apvts;
  DirectoryList* dirList;
  std::unique_ptr<FileListComponent> file_list_component
  { std::make_unique<FileListComponent>(*dirList->getDirectoryContentsList(), apvts) };
};

} // namespace iNVOXRecords::gui