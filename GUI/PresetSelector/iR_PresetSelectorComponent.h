#pragma once

#include "iR_PresetSelectorLookAndFeel.h"
#include "../../iR_ProcessorUtil/iR_ProcessorUtil.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

using APVTS = AudioProcessorValueTreeState;

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_DirectoryList class
//----------------------------------------------------------------------------------------------------------------------
class iR_DirectoryList
{
public:
  // constructor
  iR_DirectoryList();

  // getter
  DirectoryContentsList* getDirectoryContentsList() { return &list; }

private:
  TimeSliceThread thread = TimeSliceThread("preset_read_thread");
  WildcardFileFilter filter = WildcardFileFilter("*.irps", "", "iNVOXRecords preset file.");
  DirectoryContentsList list = DirectoryContentsList(&filter, thread);
};

//----------------------------------------------------------------------------------------------------------------------
// iR_FileListComponentWrapper class
//----------------------------------------------------------------------------------------------------------------------
class iR_FileListComponent : public FileListComponent, public FileBrowserListener
{
public:
  // constructor
  iR_FileListComponent(DirectoryContentsList& directoryContentList, APVTS* apvts);

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
// iR_PresetMenuButton class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetMenuButton : public Button
{
public:
  // constructor
  iR_PresetMenuButton(iR_UserProperties* user_properties, APVTS* apvts, iR_DirectoryList* dir_list);

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void mouseDown(const MouseEvent& event) override;

  // getter
  PopupMenu* getPopupMenu() { return &popup_menu; }

private:
  PopupMenu popup_menu;
  iR_UserProperties* user_properties;
  APVTS* apvts;
  iR_DirectoryList* dir_list;
  std::unique_ptr<FileChooser> file_chooser;
};

//----------------------------------------------------------------------------------------------------------------------
// iR_PresetListButton class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetListButton : public Button
{
public:
  // constructor
  iR_PresetListButton(APVTS* apvts, iR_DirectoryList* dir_list);

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  void mouseDown(const MouseEvent& event) override;

  // getter
  iR_FileListComponent* getFileListComponent() { return file_list_component.get(); }

  // setter
  void setPosition(Rectangle<int>& rect) { setBounds(rect); }

private:
  APVTS* apvts;
  iR_DirectoryList* dir_list;
  std::unique_ptr<iR_FileListComponent> file_list_component 
    = std::make_unique<iR_FileListComponent>(*dir_list->getDirectoryContentsList(), apvts);
};

}
}