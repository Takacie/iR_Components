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

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui {

class ResizeInterface
{
public:
  // constructor
  ResizeInterface(const float& scale) :
    scale(scale)
  {}

  // getter
  const float& getScale() const noexcept { return scale; }

  int getInitX() const noexcept { return initBounds.getX(); }
  int getInitY() const noexcept { return initBounds.getY(); }
  int getInitWidth() const noexcept { return initBounds.getWidth(); }
  int getInitHeight() const noexcept { return initBounds.getHeight(); }

  Rectangle<int> getInitBounds() const noexcept { return initBounds; }

  int getScaledX() const noexcept { return initBounds.getX() * scale; }
  int getScaledY() const noexcept { return initBounds.getY() * scale; }
  int getScaledWidth() const noexcept { return initBounds.getWidth() * scale; }
  int getScaledHeight() const noexcept { return initBounds.getHeight() * scale; }

  Rectangle<int> getScaledBounds() const noexcept
  {
    return Rectangle<int>(getScaledX(), getScaledY(), getScaledWidth(), getScaledHeight());
  }

  // setter
  void setInitBounds(const Rectangle<int>& newBounds) noexcept
  {
    initBounds = newBounds;
  }

  void setInitBounds(int x, int y, int width, int height) noexcept
  {
    initBounds = Rectangle<int>(x, y, width, height);
  }

  void setInitX(int x) { initBounds.setX(x); }
  void setInitY(int y) { initBounds.setY(y); }
  void setInitWidth(int width) { initBounds.setWidth(width); }
  void setInitHeight(int height) { initBounds.setHeight(height); }

private:
  // member
  const float& scale;
  Rectangle<int> initBounds;
};

} // namespace iNVOXRecords::gui