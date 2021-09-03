#ifndef BODY_DETECTOR_
#define BODY_DETECTOR_

#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <benchmark.h>
#include <cpu.h>
#include <datareader.h>
#include <gpu.h>
#include <net.h>
#include <platform.h>

#if __ARM_NEON
#include <arm_neon.h>
#endif  // __ARM_NEON

#include "daisykitsdk/common/types.h"

class BodyDetector {
 private:
  const int _input_width = 320;
  const int _input_height = 320;
  ncnn::Mutex _lock;
  ncnn::Net* _model = 0;

 public:
  BodyDetector(const std::string& param_file, const std::string& weight_file);
  void load_model(const std::string& param_file,
                  const std::string& weight_file);
#ifdef __ANDROID__
  BodyDetector(AAssetManager* mgr, const std::string& param_file,
               const std::string& weight_file);
  void load_model(AAssetManager* mgr, const std::string& param_file,
                  const std::string& weight_file);
#endif

  std::vector<Object> detect(cv::Mat& image);
};

#endif