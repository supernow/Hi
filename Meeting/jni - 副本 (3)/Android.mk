# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

######################build webrtc#####################
include $(CLEAR_VARS)
LOCAL_MODULE    := meeting
LOCAL_ARM_MODE := arm

LOCAL_CPP_EXTENSION := .cxx .cpp .cc

LOCAL_SRC_FILES = neteq.cpp \
                  webrtcWraper.cpp
				  
LOCAL_SRC_FILES += \
Meeting.c \
webrtcConf.cpp \
webrtcConfPriv.cpp

LOCAL_SRC_FILES += \
		webrtc/system_wrappers/cpu_features_arm.c

LOCAL_SRC_FILES += \
		webrtc/utility/delay_estimator.c \
		webrtc/utility/delay_estimator_wrapper.c \
		webrtc/utility/fft4g.c \
		webrtc/utility/ring_buffer.c
		
LOCAL_SRC_FILES += \
    webrtc/signal_processing/auto_corr_to_refl_coef.c \
    webrtc/signal_processing/auto_correlation.c \
    webrtc/signal_processing/complex_fft.c \
    webrtc/signal_processing/copy_set_operations.c \
    webrtc/signal_processing/cross_correlation.c \
    webrtc/signal_processing/division_operations.c \
    webrtc/signal_processing/dot_product_with_scale.c \
    webrtc/signal_processing/downsample_fast.c \
    webrtc/signal_processing/energy.c \
    webrtc/signal_processing/filter_ar.c \
    webrtc/signal_processing/filter_ma_fast_q12.c \
    webrtc/signal_processing/get_hanning_window.c \
    webrtc/signal_processing/get_scaling_square.c \
    webrtc/signal_processing/ilbc_specific_functions.c \
    webrtc/signal_processing/levinson_durbin.c \
    webrtc/signal_processing/lpc_to_refl_coef.c \
    webrtc/signal_processing/min_max_operations.c \
    webrtc/signal_processing/randomization_functions.c \
    webrtc/signal_processing/real_fft.c \
    webrtc/signal_processing/refl_coef_to_lpc.c \
    webrtc/signal_processing/resample.c \
    webrtc/signal_processing/resample_48khz.c \
    webrtc/signal_processing/resample_by_2.c \
    webrtc/signal_processing/resample_by_2_internal.c \
    webrtc/signal_processing/resample_fractional.c \
    webrtc/signal_processing/spl_init.c \
    webrtc/signal_processing/spl_sqrt.c \
    webrtc/signal_processing/spl_version.c \
    webrtc/signal_processing/splitting_filter.c \
    webrtc/signal_processing/sqrt_of_one_minus_x_squared.c \
    webrtc/signal_processing/vector_scaling_operations.c \
    webrtc/signal_processing/filter_ar_fast_q12_armv7.S \
    webrtc/signal_processing/complex_bit_reverse_arm.S \
    webrtc/signal_processing/spl_sqrt_floor_arm.S
		
LOCAL_SRC_FILES += \
    webrtc/aec/echo_cancellation.c \
    webrtc/aec/aec_resampler.c \
    webrtc/aec/aec_core.c \
    webrtc/aec/aec_rdft.c

LOCAL_SRC_FILES += \
    webrtc/ns/noise_suppression.c \
    webrtc/ns/ns_core.c
    
LOCAL_SRC_FILES += \
		webrtc/vad/vad_core.c \
		webrtc/vad/vad_filterbank.c \
		webrtc/vad/vad_gmm.c \
		webrtc/vad/vad_sp.c \
		webrtc/vad/webrtc_vad.c
		

LOCAL_SRC_FILES += \
		webrtc/agc/agc.cc \
		webrtc/agc/agc_audio_proc.cc \
		webrtc/agc/agc_manager_direct.cc \
		webrtc/agc/circular_buffer.cc \
		webrtc/agc/gmm.cc \
		webrtc/agc/histogram.cc \
		webrtc/agc/pitch_based_vad.cc \
		webrtc/agc/pitch_internal.cc \
		webrtc/agc/pole_zero_filter.cc \
		webrtc/agc/standalone_vad.cc \
		webrtc/agc/utility.cc \
		webrtc/agc/agency/analog_agc.c \
		webrtc/agc/agency/digital_agc.c

LOCAL_SRC_FILES += \
		webrtc/isac/pitch_estimator.c \
		webrtc/isac/filterbank_tables.c \
		webrtc/isac/filterbanks.c \
		webrtc/isac/intialize.c \
		webrtc/isac/filter_functions.c \
		webrtc/isac/lpc_analysis.c \
		webrtc/isac/pitch_filter.c \
		webrtc/isac/transform.c \
		webrtc/isac/fft.c
		
LOCAL_SRC_FILES += \
		webrtc/resampler/resampler.cc
    
MY_WEBRTC_COMMON_DEFS := \
    '-DWEBRTC_TARGET_PC' \
    '-DWEBRTC_LINUX' \
    '-DWEBRTC_THREAD_RR' \
    '-DWEBRTC_CLOCK_TYPE_REALTIME' \
    '-DWEBRTC_ANDROID' \
    '-DWEBRTC_POSIX' \
    '-DWEBRTC_ARCH_ARM' \
    '-DWEBRTC_ARCH_ARM_V7'


LOCAL_CFLAGS := \
    $(MY_WEBRTC_COMMON_DEFS)


LOCAL_C_INCLUDES += \
    webrtc/interface \
    webrtc/utility \
    webrtc/common \
    webrtc/signal_processing/include \
    webrtc/system_wrappers/interface \
    webrtc/ns \
    webrtc/agc \
    webrtc/agc/agency \
    webrtc/agc/agency/include \
    webrtc/isac \
    webrtc/resampler \
    webrtc/resampler/include \
    webrtc/vad \
    webrtc/vad/include

LOCAL_LDLIBS    += -llog
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)

#####################################################

