#include "iR_UserProperties.h"

namespace iNVOXRecords::utility {
//----------------------------------------------------------------------------------------------------------------------
// UserProperties implementation
//----------------------------------------------------------------------------------------------------------------------
iR_UserProperties::iR_UserProperties(const String& applicationName)
{
  // properties file will be stored in C:/Users/{User}/AppData/Roaming/iNVOX Records/{applicationName}.iconf
  options.applicationName = applicationName;
  options.folderName = juce::String("iNVOX Records");
  options.filenameSuffix = juce::String(".irconf");
  options.osxLibrarySubFolder = juce::String("Application Support");
  setStorageParameters(options);
  user_property = getUserSettings();
}

void iR_UserProperties::setScaleProperty(float newScale, bool withSave)
{
  if (user_property) {
    user_property->setValue(StringRef("scale"), newScale);

    if (withSave) user_property->save();
  }
}

} // namespace iNVOXRecords::utility