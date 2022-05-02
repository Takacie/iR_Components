#pragma once

#include "../../ThirdParty/muFFT/fft.h"

namespace iNVOXRecords {

class FFT
{
public:
  FFT(int initOrder)
    : fftOrder{ initOrder }
  {}

private:
  int fftOrder;
  int fftSize{ 1 << fftOrder };
  std::unique_ptr<mufft_plan_1d> mufft_plan{ std::make_unique<mufft_plan_1d>(mufft_create_plan_1d_r2c(fftSize, 1)) };
};

} // namespace iNVOXRecords