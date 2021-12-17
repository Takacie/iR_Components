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

#include "EQ_GraphicController.h"
#include "../Handle/EQ_Handle.h"

namespace iNVOXRecords::gui::equalizer {
//----------------------------------------------------------------------------------------------------------------------
// EqGraphicController implementation
//----------------------------------------------------------------------------------------------------------------------
EqGraphicController::EqGraphicController(EqProcessor& processor, const int initWidth, const int initHeight) :
  processor(processor),
  init_width(initWidth),
  init_height(initHeight)
{
}

void EqGraphicController::removeHandle(int index)
{
  handles[index].reset();
  repaint();
}

void EqGraphicController::paint(Graphics& g)
{
  g.setColour(Colour(10, 10, 15));
  g.fillAll();
  drawGridAndDecibels(g);

  g.setColour(Colour(250, 250, 255));
  Point<float> tmp;
  for (auto i = 0.0f; i < 1.0f; i += 0.001f) {
    const auto x = getWidth() * i;
    const auto f = mapToLog10<float>(i, MIN_FREQ, MAX_FREQ);
    const auto samplerate = processor.getSamplerate();

    // calculate magnitude
    auto magnitude = 1.0f;
    magnitude *= processor.getLPFCoeffs()->getMagnitudeForFrequency(f, samplerate);
    magnitude *= processor.getHPFCoeffs()->getMagnitudeForFrequency(f, samplerate);
    for (auto i = 0; i < MAX_FILTERS; ++i) {
      if (handles[i]) {
        magnitude *= processor.getCoeffs(i)->getMagnitudeForFrequency(f, samplerate);
      }
    }

    // set min and max
    float min = 0.0f, max = 0.0f;
    auto setMinMax = [&min, &max](float newMin, float newMax) {
      min = newMin; max = newMax;
    };
    switch (decibelLimit)
    {
    case DL::k6dB:  setMinMax(-6.0f, 6.0f);  break;
    case DL::k12dB: setMinMax(-12.0f, 12.0f); break;
    case DL::k24dB: setMinMax(-24.0f, 24.0f); break;
    }

    // calculate y
    const auto magnitude_decibels = Decibels::gainToDecibels(magnitude);
    const auto half_y = getHeight() / 2.0f;
    auto y = 0.0f;
    if (magnitude >= 1.0f) { // over 0dB
      y = (1 - magnitude_decibels / max) * half_y;
    }
    else { // under 0dB
      y = (magnitude_decibels / min) * half_y + half_y;
    }

    // draw line
    auto line_w = getHeight() / 200.0f; // line width
    if (!tmp.isOrigin()) g.drawLine(tmp.x, tmp.y, x, y, line_w);

    tmp = Point<float>(x, y); // for next loop
  }
}

void EqGraphicController::mouseDoubleClick(const MouseEvent& event)
{
  for (auto i = 0; i < MAX_FILTERS; ++i) {
    if (!handles[i]) {
      const auto pos = event.getPosition();
      const auto freq = mapToLog10<float>(pos.x / (init_width * current_scale), MIN_FREQ, MAX_FREQ);

      // generate new handle
      handles[i] = std::make_unique<EqHandle>(*this, processor, pos, i);
      handles[i]->setHandleColour(HANDLE_COLORS[i]);
      processor.prepareFilter(i);

      addAndMakeVisible(*handles[i]);
      repaint();
      break;
    }
  }
}

void EqGraphicController::setPosition(int x, int y, float scale)
{
  current_scale = scale;
  generateLpfAndHpfHandles(scale);
  setBounds(x * scale, y * scale, init_width * scale, init_height * scale);
}

} // namespace iNVOXRecords::gui::equalizer