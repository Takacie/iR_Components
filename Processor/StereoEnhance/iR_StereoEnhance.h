#pragma once

namespace iNVOXRecords::gui {
static void setStereoEnhance(float width, float* in_L, float* in_R)
{
  float tmp = 1.0f / std::fmax(1.0f + width, 2.0f);
  float mid = (*in_L + *in_R) * tmp;
  float side = (*in_R - *in_L) * width * tmp;
  *in_L = mid - side;
  *in_R = mid + side;
}
} // namespace iNVOXRecords::gui