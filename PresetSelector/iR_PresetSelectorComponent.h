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
  iR_DirectoryList()
  {
    list.setDirectory(File::getCurrentWorkingDirectory(), false, true);
    thread.startThread();
  }

  // getter
  DirectoryContentsList* getDirectoryContentsList() { return &list; }

private:
  TimeSliceThread thread = TimeSliceThread("preset-read-thread");
  WildcardFileFilter filter = WildcardFileFilter("*.irps", "", "iNVOXRecords preset file.");
  DirectoryContentsList list = DirectoryContentsList(&filter, thread);
};

//----------------------------------------------------------------------------------------------------------------------
// iR_PresetMenuButton class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetMenuButton : public Button
{
public:
  // constructor
  iR_PresetMenuButton(iR_UserProperties* user_properties, APVTS* apvts, iR_DirectoryList* dir_list) :
    Button(""),
    user_properties(user_properties),
    apvts(apvts),
    dir_list(dir_list)
  {
    File initial_location = File();
    if (user_properties->getUserSettings()->getValue("preset_dir").isNotEmpty()) {
      initial_location = File::File(user_properties->getUserSettings()->getValue("preset_dir"));
    }
    dir_list->getDirectoryContentsList()->setDirectory(initial_location, false, true);
    file_chooser = std::make_unique<FileChooser>("Select the location where you want to save the presets.",
      initial_location, "*.irps", true, false, this);

    popup_menu.addItem(1, "Save preset");
    popup_menu.addItem(2, "Change directory");
    popup_menu.addItem(3, "Refresh");
  }

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
  {
    const Rectangle<int> b = getLocalBounds();
    const int w = getWidth();
    const int h = getHeight();
    const int x = b.getX();
    const int y = b.getY();
    const int r = h / 2;

    // draw background
    if (shouldDrawButtonAsHighlighted) g.setColour(Colour(70, 70, 75));
    else g.setColour(Colour(50, 50, 55));
    g.fillEllipse(x + w - h, y, h, h);
    g.fillRect(x, y, w - r, h);

    // draw signature
    g.setColour(Colours::white);
    const float sign_w = w * 0.5f;
    const float sign_h = h * 0.05f;
    const float sign_x = x + w * 0.15f;
    Rectangle<float> sign_rect = Rectangle<float>(sign_x, 0, sign_w, sign_h);
    g.fillRect(sign_rect.withY(y + h * 0.3f - sign_h / 2));
    g.fillRect(sign_rect.withY(y + h * 0.5f - sign_h / 2));
    g.fillRect(sign_rect.withY(y + h * 0.7f - sign_h / 2));
  }

  void mouseDown(const MouseEvent& event) override
  {
    Button::mouseDown(event);
    Point<int> click_position = event.getPosition();
    popup_menu.showMenuAsync(PopupMenu::Options()
      .withPreferredPopupDirection(PopupMenu::Options::PopupDirection::downwards)
      .withTargetComponent(this)
      .withMinimumWidth(getWidth() * 3.f)
      .withStandardItemHeight(getHeight() * 0.8f),
      [&](int choice) {
        int flag = 0;
        switch (choice)
        {
        case 1:
          flag = FileBrowserComponent::FileChooserFlags::saveMode;
          file_chooser->launchAsync(flag, [&](const FileChooser& chooser) {
            File save_file = chooser.getResult();
            if (save_file.getFullPathName().isNotEmpty()) {
              std::unique_ptr<XmlElement> preset_xml = apvts->copyState().createXml();
              preset_xml->setAttribute("PluginName", PROJECT_NAME);
              preset_xml->writeTo(chooser.getResult());
            }
            });
          break;
        case 2:
          flag = FileBrowserComponent::FileChooserFlags::openMode
            | FileBrowserComponent::FileChooserFlags::canSelectDirectories;
          file_chooser->launchAsync(flag, [&](const FileChooser& chooser) {
            File new_dir = chooser.getResult();
            if (new_dir.exists()) {
              user_properties->getUserSettings()->setValue("preset_dir", new_dir.getFullPathName());
            }
            });
          break;
        case 3:
          dir_list->getDirectoryContentsList()->refresh();
          break;
        default:
          break;
        }
      });
  }

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
// iR_FileListComponentWrapper class
//----------------------------------------------------------------------------------------------------------------------
class iR_FileListComponentWrapper : public FileListComponent, public FileBrowserListener
{
public:
  // constructor
  iR_FileListComponentWrapper(DirectoryContentsList& directoryContentList, APVTS* apvts) :
    FileListComponent(directoryContentList),
    apvts(apvts)
  {
    setAlwaysOnTop(true);
    addListener(this);
    setFocusContainerType(FocusContainerType::focusContainer);
    setColour(backgroundColourId, Colour(50, 50, 55));
  }

  // override
  void selectionChanged() override {}
  void fileClicked(const File& file, const MouseEvent& e) override {}
  void browserRootChanged(const File& newRoot) override {}
  void fileDoubleClicked(const File& file) override
  {
    if (file.getFullPathName().isNotEmpty()) {
      setVisible(false);
      std::unique_ptr<XmlElement> preset_xml = parseXML(file);
      if (preset_xml->hasAttribute("PluginName") && preset_xml->getStringAttribute("PluginName") == PROJECT_NAME) {
        ValueTree preset_tree = ValueTree::fromXml(*preset_xml);
        apvts->replaceState(preset_tree);
        preset_name = file.getFileNameWithoutExtension();
      }
    }
  }
  void inputAttemptWhenModal() override
  {
    exitModalState(0);
    setVisible(false);
  }


  // getter
  String getPresetName() const { return preset_name; }

private:
  String preset_name = "(default)";
  APVTS* apvts;
};

//----------------------------------------------------------------------------------------------------------------------
// iR_PresetListButton class
//----------------------------------------------------------------------------------------------------------------------
class iR_PresetListButton : public Button
{
public:
  // constructor
  iR_PresetListButton(APVTS* apvts, iR_DirectoryList* dir_list) :
    Button(""),
    apvts(apvts),
    dir_list(dir_list)
  {
    file_list_component->setVisible(false);
  }

  // override
  void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
  {
    const Rectangle<int> b = getLocalBounds();
    const int w = getWidth();
    const int h = getHeight();
    const int x = b.getX();
    const int y = b.getY();
    const int r = h / 2;

    // background color
    if (shouldDrawButtonAsHighlighted) g.setColour(Colour(70, 70, 75));
    else g.setColour(Colour(50, 50, 55));

    // draw background
    g.fillEllipse(x, y, h, h);
    g.fillRect(x + r, y, w, h);

    g.setFont(Font(getHeight() * 0.6f, Font::plain));
    g.setColour(Colours::white);
    const Rectangle<int> text_rect(x + r / 2, y, w, h);
    g.drawText(file_list_component->getPresetName(), text_rect, Justification::centred);
  }

  // getter
  iR_FileListComponentWrapper* getFileListComponent() { return file_list_component.get(); }

  // setter
  void setPosition(Rectangle<int> rect)
  {
    setBounds(rect);
  }

  void mouseDown(const MouseEvent& event) override
  {
    Button::mouseDown(event);
    file_list_component->updateContent();
    file_list_component->setVisible(!file_list_component->isVisible());
    file_list_component->enterModalState();
  }

private:
  APVTS* apvts;
  iR_DirectoryList* dir_list;
  std::unique_ptr<iR_FileListComponentWrapper> file_list_component = std::make_unique<iR_FileListComponentWrapper>(*dir_list->getDirectoryContentsList(), apvts);
};

}
}