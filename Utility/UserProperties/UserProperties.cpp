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

#include "UserProperties.h"

namespace iNVOXRecords::utility {
//----------------------------------------------------------------------------------------------------------------------
// UserProperties implementation
//----------------------------------------------------------------------------------------------------------------------
UserProperties::UserProperties(const String& applicationName)
{
  // properties file will be stored in C:/Users/{User}/AppData/Roaming/iNVOX Records/{applicationName}.iconf
  options.applicationName = applicationName;
  options.folderName = juce::String("iNVOX Records");
  options.filenameSuffix = juce::String(".irconf");
  options.osxLibrarySubFolder = juce::String("Application Support");
  setStorageParameters(options);
  userProperty = getUserSettings();
}

void UserProperties::setScaleProperty(float newScale, bool withSave)
{
  if (userProperty) {
    userProperty->setValue(StringRef("scale"), newScale);

    if (withSave) userProperty->save();
  }
}

} // namespace iNVOXRecords::utility