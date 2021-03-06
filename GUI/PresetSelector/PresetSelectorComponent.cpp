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

#include "PresetSelectorComponent.h"

namespace iNVOXRecords::gui {
//----------------------------------------------------------------------------------------------------------------------
// DirectoryList implementation
//----------------------------------------------------------------------------------------------------------------------
DirectoryList::DirectoryList()
{
  list.setDirectory(File::getCurrentWorkingDirectory(), false, true);
  thread.startThread();
}

//----------------------------------------------------------------------------------------------------------------------
// FileListComponentWrapper implementation
//----------------------------------------------------------------------------------------------------------------------
FileListComponent::FileListComponent(DirectoryContentsList& directoryContentList, APVTS& apvts) :
  juce::FileListComponent(directoryContentList),
  apvts(apvts)
{
  setAlwaysOnTop(true);
  addListener(this);
  setFocusContainerType(FocusContainerType::focusContainer);
  setColour(backgroundColourId, Colour(50, 50, 55));
  setColour(outlineColourId, Colour(200, 200, 205));
  setOutlineThickness(1);
}

void FileListComponent::fileDoubleClicked(const File& file)
{
  if (file.getFullPathName().isNotEmpty()) {
    setVisible(false);
    auto presetXml = parseXML(file);
    if (presetXml->hasAttribute("PluginName") && presetXml->getStringAttribute("PluginName") == PROJECT_NAME) {
      auto preset_tree = ValueTree::fromXml(*presetXml);
      apvts.replaceState(preset_tree);
      presetName = file.getFileNameWithoutExtension();
    }
    else {
      getLookAndFeel().playAlertSound();
    }
  }
}
void FileListComponent::inputAttemptWhenModal()
{
  exitModalState(0);
  setVisible(false);
}

//----------------------------------------------------------------------------------------------------------------------
// PresetMenuButton implementation
//----------------------------------------------------------------------------------------------------------------------
PresetMenuButton::PresetMenuButton(UserProperties* userProperties, APVTS& apvts, DirectoryList* dirList) :
  Button("PresetMenuButton"),
  userProperties(userProperties),
  apvts(apvts),
  dirList(dirList)
{
  File initial_location = File();
  if (userProperties->getUserSettings()->getValue("preset_dir").isNotEmpty())
    initial_location = File::File(userProperties->getUserSettings()->getValue("preset_dir"));

  dirList->getDirectoryContentsList()->setDirectory(initial_location, false, true);
  fileChooser = std::make_unique<FileChooser>("Select the location where you want to save the presets.",
                                               initial_location, "*.irps", true, false, this);

  popupMenu.addItem(1, "Save preset");
  popupMenu.addItem(2, "Change directory");
  popupMenu.addItem(3, "Refresh");
}

void PresetMenuButton::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
  const auto b = getLocalBounds();
  const auto w = getWidth();
  const auto h = getHeight();
  const auto x = b.getX();
  const auto y = b.getY();
  const auto r = h / 2;

  // draw background
  if (shouldDrawButtonAsHighlighted) g.setColour(Colour(70, 70, 75));
  else g.setColour(Colour(50, 50, 55));
  g.fillEllipse(x + w - h, y, h, h);
  g.fillRect(x, y, w - r, h);

  // draw signature
  g.setColour(Colours::white);
  const auto sign_w = w * 0.5f;
  const auto sign_h = h * 0.05f;
  const auto sign_x = x + w * 0.15f;
  Rectangle<float> sign_rect(sign_x, 0, sign_w, sign_h);
  g.fillRect(sign_rect.withY(y + h * 0.3f - sign_h / 2));
  g.fillRect(sign_rect.withY(y + h * 0.5f - sign_h / 2));
  g.fillRect(sign_rect.withY(y + h * 0.7f - sign_h / 2));
}

void PresetMenuButton::mouseDown(const MouseEvent& event)
{
  Button::mouseDown(event);
  auto click_position = event.getPosition();
  popupMenu.showMenuAsync(PopupMenu::Options()
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
        fileChooser->launchAsync(flag, [&](const FileChooser& chooser) {
          File save_file = chooser.getResult();
          if (save_file.getFullPathName().isNotEmpty()) {
            std::unique_ptr<XmlElement> presetXml = apvts.copyState().createXml();
            presetXml->setAttribute("PluginName", PROJECT_NAME);
            presetXml->writeTo(chooser.getResult());
          }
          });
        break;
      case 2:
        flag = FileBrowserComponent::FileChooserFlags::openMode
          | FileBrowserComponent::FileChooserFlags::canSelectDirectories;
        fileChooser->launchAsync(flag, [&](const FileChooser& chooser) {
          File new_dir = chooser.getResult();
          if (new_dir.exists()) {
            userProperties->getUserSettings()->setValue("preset_dir", new_dir.getFullPathName());
          }
          });
        break;
      case 3:
        dirList->getDirectoryContentsList()->refresh();
        break;
      default:
        break;
      }
    });
}

//----------------------------------------------------------------------------------------------------------------------
// PresetListButton implementation
//----------------------------------------------------------------------------------------------------------------------
PresetListButton::PresetListButton(APVTS& apvts, DirectoryList* dirList) :
  Button("PresetListButton"),
  apvts(apvts),
  dirList(dirList)
{
  fileListComponent->setVisible(false);
}

void PresetListButton::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
  const auto b = getLocalBounds();
  const auto w = getWidth();
  const auto h = getHeight();
  const auto x = b.getX();
  const auto y = b.getY();
  const auto r = h / 2;

  // background color
  if (shouldDrawButtonAsHighlighted) g.setColour(Colour(70, 70, 75));
  else g.setColour(Colour(50, 50, 55));

  // draw background
  g.fillEllipse(x, y, h, h);
  g.fillRect(x + r, y, w, h);

  g.setFont(Font(getHeight() * 0.6f, Font::plain));
  g.setColour(Colours::white);
  const Rectangle<int> text_rect(x + r / 2, y, w, h);
  g.drawText(fileListComponent->getPresetName(), text_rect, Justification::centred);
}

void PresetListButton::mouseDown(const MouseEvent& event)
{
  Button::mouseDown(event);
  fileListComponent->updateContent();
  fileListComponent->setVisible(!fileListComponent->isVisible());
  fileListComponent->enterModalState();
}

} // namespace iNVOXRecords::gui