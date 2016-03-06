#pragma once
#include "ofMain.h"

const int kMaxTouch = 8;

const int kNumTimeSlices = 256;
const int kNumBins = 1024;
const int kHalfNumBins = kNumBins / 2;
const int kHalfNumTimeSlices = kNumTimeSlices / 2;
const int kNumVertices = kNumBins * kNumTimeSlices;
const int kSampleRate = 44100;
const int kNumInput = 2;
const int kNumOutput = 2;

const float kNormalLineWidth = 8;
const float kStemLineWidth = 6;
const float kWaveformLineWidth = 4;
const float kThinLineWidth = 1;
const float kCircleSize = 100;
const float kSmallCircleSize = 50;
const float kCentroidSize = 150;
const float kNormalFontSize = 18;
const float kInterpolationRatio = 0.1;
const float kForceInfluence = 0.65;
const float kReleaseRation = 0.08;
const int kBlockSize = 512;
const int kTicksPerBuffer = 8;
const int kArraySize = 512;
const float kWaveformScale = 1.0/kArraySize;
const float kCaptionMargin = 18;
const float kNearEnoughThreshold = 2.0;
const float kArcRadius = 150;
const float kRippleAnimationStep = 0.1;
const float kRippleSize = 8;
