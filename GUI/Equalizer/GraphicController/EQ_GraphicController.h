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

#include "../Handle/EQ_Handle.h"
#include "../../../Processor/Equalizer/EQ_Processor.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace iNVOXRecords::gui::equalizer {

using namespace processor::equalizer;

class EqHandle;

//----------------------------------------------------------------------------------------------------------------------
// constants
//----------------------------------------------------------------------------------------------------------------------
static const std::array<Colour, MAX_FILTERS> HANDLE_COLORS = {
  Colour(22.4375f / 360 * 0, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 15, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 1, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 14, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 2, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 13, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 3, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 12, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 4, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 11, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 5, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 10, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 6, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 9, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 7, 0.5f, 0.75f, 1.0f),
  Colour(22.4375f / 360 * 8, 0.5f, 0.75f, 1.0f),
};

//----------------------------------------------------------------------------------------------------------------------
// enums
//----------------------------------------------------------------------------------------------------------------------
enum class DecibelLimits {
  k6dB, k12dB, k24dB 
};

//----------------------------------------------------------------------------------------------------------------------
// aliases
//----------------------------------------------------------------------------------------------------------------------
using EqHandlePtr = std::unique_ptr<EqHandle>;
using DL = DecibelLimits;

//----------------------------------------------------------------------------------------------------------------------
// EqGraphicController class
//----------------------------------------------------------------------------------------------------------------------
class EqGraphicController : public juce::Component
{
public:
  // constructor
  EqGraphicController(EqProcessor& processor, const int initWidth, const int initHeight);

  // common method
  void removeHandle(int index);

  // override
  void paint(Graphics& g) override;

  void mouseDoubleClick(const MouseEvent& event) override;

  // getter
  float getCurrentScale() const { return current_scale; }
  DL getDecibelLimit() const { return decibelLimit; }

  // setter
  void setPosition(int x, int y, float scale);

private:
  // member
  EqProcessor& processor;
  const int init_width, init_height;
  float current_scale = 1.0f;
  DL decibelLimit = DL::k12dB;

  EqHandlePtr lpf;
  EqHandlePtr hpf;
  std::array<EqHandlePtr, MAX_FILTERS> handles;

  // helper
  void drawGridAndDecibels(Graphics& g)
  {
    const auto w = getWidth();
    const auto h = getHeight();
    const auto text_w = 30 * current_scale;
    const auto text_h = 10 * current_scale;
    const auto top_m = 2 * current_scale;
    const Colour text_c(100, 100, 105);
    const Colour line_c(30, 30, 35);
    auto decibelText = [&g, h, text_w, text_h, top_m, text_c, this](float y_ratio, int num) {
      g.setColour(text_c);
      g.drawText(String(num) + " dB", 0, h * y_ratio + top_m, text_w, text_h, Justification::centredRight);
    };
    auto drawHLine = [&g, w, h, line_c](float y_ratio) {
      g.setColour(line_c);
      g.drawHorizontalLine(h * y_ratio - 1, 0, w);
    };

    g.setFont(Font(text_h));

    int num = 0;
    switch (decibelLimit)
    {
    case DL::k6dB:
      decibelText(0, 6);
      decibelText(0.5f, 0);
      drawHLine(0.5f);
      break;
    case DL::k12dB:
      num = 12;
      for (float ratio = 0.0f; ratio < 1.0f; ratio += 0.25f, num -= 6) {
        decibelText(ratio, num);
        drawHLine(ratio);
      }
      break;
    case DL::k24dB:
      num = 24;
      for (float ratio = 0.0f; ratio < 1.0f; ratio += 0.125f, num -= 6) {
        decibelText(ratio, num);
        drawHLine(ratio);
      }
      break;
    }
  }

  void generateLpfAndHpfHandles(float scale)
  {
    if (!lpf) {
      const Point<int> lpf_pos(init_width * scale * 0.8f, init_height * scale * 0.5f);
      const auto freq = mapToLog10<float>(lpf_pos.x / (init_width * scale), MIN_FREQ, MAX_FREQ);
      lpf = std::make_unique<EqHandle>(*this, processor, lpf_pos, -1, FT::LowPass);
      lpf->setHandleColour(Colour(150, 150, 155));
      processor.updateFilterSettings(FT::LowPass, freq);
      addAndMakeVisible(*lpf);
    }
    if (!hpf) {
      const Point<int> hpf_pos(init_width * scale * 0.2f, init_height * scale * 0.5f);
      const auto freq = mapToLog10<float>(hpf_pos.x / (init_width * scale), MIN_FREQ, MAX_FREQ);
      hpf = std::make_unique<EqHandle>(*this, processor, hpf_pos, -1, FT::HighPass);
      hpf->setHandleColour(Colour(0, 0, 5));
      processor.updateFilterSettings(FT::HighPass, freq);
      addAndMakeVisible(*hpf);
    }
  }
};

} // namespace iNVOXRecords::gui::equalizer