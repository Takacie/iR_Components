#include "iR_EQ_Handle.h"
#include "../GraphicController/iR_EQ_GraphicController.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace iNVOXRecords::gui::equalizer {
//----------------------------------------------------------------------------------------------------------------------
// iR_EqHandle implementation
//----------------------------------------------------------------------------------------------------------------------
iR_EqHandle::iR_EqHandle(iR_EqGraphicController& parent, iR_EqProcessor& processor,
  const Point<int>& initialPos, int index, FT type) :
  parent(parent),
  processor(processor),
  handleIndex(index),
  type(type)
{
  const auto handle_w = WIDTH * parent.getCurrentScale(); // handle width
  const auto half_w = handle_w / 2;

  setBounds(initialPos.x - half_w, initialPos.y - half_w, handle_w, handle_w);

  constrainer.setMinimumOnscreenAmounts(half_w, half_w, half_w, half_w);
  setBroughtToFrontOnMouseClick(true);

  if (!isLPForHPF()) {
    processor.generateFilter(type, calcFrequency(), index);
    resetFilterSettings();

    initFreqAttachment();
  }
}

void iR_EqHandle::paint(Graphics& g)
{
  g.setColour(Colours::white);
  g.fillEllipse(getLocalBounds().toFloat());

  g.setColour(colour);
  g.fillEllipse(getLocalBounds().reduced(WIDTH * 0.15f * parent.getCurrentScale()).toFloat());
}

void iR_EqHandle::parentSizeChanged()
{
  if (!isValidRelativePos()) {
    updateRelativePos();
  }

  const auto handle_w = WIDTH * parent.getCurrentScale(); // handle width
  const auto half_w = handle_w / 2;

  setSize(handle_w, handle_w);
  setCentreRelative(relativePos[0], relativePos[1]);

  constrainer.setMinimumOnscreenAmounts(half_w, half_w, half_w, half_w);
}

void iR_EqHandle::mouseDoubleClick(const MouseEvent& event)
{
  if (isLPForHPF()) return;
  processor.removeFilter(handleIndex);
  parent.removeHandle(handleIndex);
}

void iR_EqHandle::mouseDrag(const MouseEvent& event)
{
  dragger.dragComponent(this, event, &constrainer);

  if (isLPForHPF()) {
    Rectangle<int> b(getBounds());
    b.setCentre(b.getCentreX(), getParentHeight() / 2);
    constrainer.setBoundsForComponent(this, b, false, false, false, false);
    resetFilterSettings();
  }
  else {
    resetFilterSettings(processor.getQ(handleIndex));
  }

  const auto value = calcFrequency();
  if (freqAttachment) freqAttachment->setValueAsPartOfGesture(value);
  parent.repaint();
}

void iR_EqHandle::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{
  auto q = processor.getQ(handleIndex);
  const auto is_q_small = MIN_Q <= q && q < 1.0f;
  const auto degree = is_q_small ? 0.075f : 0.3f;

  if (wheel.deltaY > 0.0f) q = jmin(q + degree, MAX_Q);
  else q = jmax(q - degree, MIN_Q);

  resetFilterSettings(q);
  parent.repaint();
}

void iR_EqHandle::resetFilterSettings(const float q)
{
  const auto f = calcFrequency();
  const auto gain = calcGainFactor();
  switch (type)
  {
  case FT::LowPass:
    processor.updateFilterSettings(FT::LowPass, f);
    break;
  case FT::HighPass:
    processor.updateFilterSettings(FT::HighPass, f);
    break;
  case FT::Peak:
    processor.updateFilterSettings(FT::Peak, f, handleIndex, q, gain);
    break;
  case FT::BandPass:
    processor.updateFilterSettings(FT::BandPass, f, handleIndex);
    break;
  case FT::LowShelf:
    processor.updateFilterSettings(FT::LowShelf, f, handleIndex);
    break;
  case FT::HighShelf:
    processor.updateFilterSettings(FT::HighShelf, f, handleIndex);
    break;
  case FT::Notch:
    processor.updateFilterSettings(FT::Notch, f, handleIndex);
    break;
  default:
    break;
  }
}

float iR_EqHandle::calcGainFactor()
{
  const auto y = getBounds().getCentreY();
  const auto half_h = static_cast<float>(parent.getHeight()) / 2;
  const auto decibel_limit = parent.getDecibelLimit();

  float min = 0.0f, max = 0.0f;
  auto setMinMax = [&min, &max](float newMin, float newMax) { 
    min = newMin; max = newMax;
  };
  switch (decibel_limit)
  {
  case DL::k6dB:  setMinMax(-6.0f, 6.0f);  break;
  case DL::k12dB: setMinMax(-12.0f, 12.0f); break;
  case DL::k24dB: setMinMax(-24.0f, 24.0f); break;
  }

  if (y < half_h) { // over 0dB
    const auto y_ratio = y / half_h;
    return Decibels::decibelsToGain((1 - y_ratio) * max);
  }
  else { // under 0dB
    const auto y_ratio = (y - half_h) / half_h;
    return Decibels::decibelsToGain(y_ratio * min);
  }
}

float iR_EqHandle::calcFrequency()
{
  const auto x = getBounds().getCentreX();
  const auto x_ratio = static_cast<float>(x) / parent.getWidth();

  return mapToLog10<float>(x_ratio, MIN_FREQ, MAX_FREQ);
}

void iR_EqHandle::updateRelativePos()
{
  const auto centrePos = getBounds().getCentre().toFloat();
  relativePos[0] = centrePos.getX() / parent.getWidth();
  relativePos[1] = centrePos.getY() / parent.getHeight();
}

} // namespace iNVOXRecords::gui::equalizer