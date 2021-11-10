#include "iR_ComboBox.h"

namespace iNVOXRecords {
namespace GUI {
//----------------------------------------------------------------------------------------------------------------------
// iR_ComboBox implementation
//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<iR_ComboBoxLookAndFeel> iR_ComboBox::lookandfeel = std::make_unique<iR_ComboBoxLookAndFeel>(Colour(132, 106, 192));

} // namespace GUI
} // namespace iNVOXRecords