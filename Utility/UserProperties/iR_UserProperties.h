#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

using namespace juce;

namespace iNVOXRecords::utility {
//----------------------------------------------------------------------------------------------------------------------
// UserProperties class
//----------------------------------------------------------------------------------------------------------------------
class iR_UserProperties : public ApplicationProperties
{
public:
  // constructor
  iR_UserProperties(const String& applicationName);

  // getter
  PropertiesFile::Options getOptions() { return options; }

  // setter
  void setScaleProperty(float newScale, bool withSave = false);

private:
  PropertiesFile* user_property;
  PropertiesFile::Options options;
};

} // namespace iNVOXRecords::utility