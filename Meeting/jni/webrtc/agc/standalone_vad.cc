/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "standalone_vad.h"

#include <assert.h>

#include "common_types.h"
//#include "audio_frame_operations.h"
#include "typedefs.h"
#include "webrtcLog.h"

namespace webrtc {

static const int kDefaultStandaloneVadMode = 3;

StandaloneVad::StandaloneVad(VadInst* vad)
    : vad_(vad),
      buffer_(),
      index_(0),
      mode_(kDefaultStandaloneVadMode) {}

StandaloneVad::~StandaloneVad() {
  WebRtcVad_Free(vad_);
}

StandaloneVad* StandaloneVad::Create() {
  VadInst* vad = NULL;
  if (WebRtcVad_Create(&vad) < 0)
    return NULL;

  int err = WebRtcVad_Init(vad);
  err |= WebRtcVad_set_mode(vad, kDefaultStandaloneVadMode);
  if (err != 0) {
    WebRtcVad_Free(vad);
    return NULL;
  }
  return new StandaloneVad(vad);
}

static const int kValidRates[] = { 8000, 16000, 32000, 48000 };
static const size_t kRatesSize = sizeof(kValidRates) / sizeof(*kValidRates);
static const int kMaxFrameLengthMs = 30;
int WebRtcVad_ValidRateAndFrameLength(int rate, int frame_length) {
  int return_value = -1;
  size_t i;
  int valid_length_ms;
  int valid_length;

  // We only allow 10, 20 or 30 ms frames. Loop through valid frame rates and
  // see if we have a matching pair.
  for (i = 0; i < kRatesSize; i++) {
    if (kValidRates[i] == rate) {
      for (valid_length_ms = 10; valid_length_ms <= kMaxFrameLengthMs;
          valid_length_ms += 10) {
        valid_length = (kValidRates[i] / 1000 * valid_length_ms);
        if (frame_length == valid_length) {
          return_value = 0;
          break;
        }
      }
      break;
    }
  }

  return return_value;
}

int StandaloneVad::AddAudio(const int16_t* data, int length) {
  if (length != kLength10Ms)
    return -1;

  if (index_ + length > kLength10Ms * kMaxNum10msFrames)
    // Reset the buffer if it's full.
    // TODO(ajm): Instead, consider just processing every 10 ms frame. Then we
    // can forgo the buffering.
    index_ = 0;

  memcpy(&buffer_[index_], data, sizeof(int16_t) * length);
  index_ += length;
  return 0;
}

int StandaloneVad::GetActivity(double* p, int length_p) {
  if (index_ == 0)
    return -1;

  const int num_frames = index_ / kLength10Ms;
  if (num_frames > length_p)
    return -1;
  assert(WebRtcVad_ValidRateAndFrameLength(kSampleRateHz, index_) == 0);

  int activity = WebRtcVad_Process(vad_, kSampleRateHz, buffer_, index_);
  if (activity < 0)
    return -1;
  else if (activity == 0)
    p[0] = 0.01;  // Arbitrary but small and non-zero.
  else
    p[0] = 0.5;  // 0.5 is neutral values when combinned by other probabilities.
  for (int n = 1; n < num_frames; n++)
    p[n] = p[0];
  // Reset the buffer to start from the beginning.
  index_ = 0;
  return activity;
}

int StandaloneVad::set_mode(int mode) {
  if (mode < 0 || mode > 3)
    return -1;
  if (WebRtcVad_set_mode(vad_, mode) != 0)
    return -1;

  mode_ = mode;
  return 0;
}

}  // namespace webrtc

