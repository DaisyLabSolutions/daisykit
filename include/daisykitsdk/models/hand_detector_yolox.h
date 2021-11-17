// Copyright 2021 The DaisyKit Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DAISYKIT_MODELS_HAND_DETECTOR_YOLOX_H_
#define DAISYKIT_MODELS_HAND_DETECTOR_YOLOX_H_

#include "daisykitsdk/common/types.h"
#include "daisykitsdk/models/image_model.h"
#include "daisykitsdk/models/ncnn_model.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

namespace daisykit {
namespace models {

/// Hand detection model
class HandDetectorYOLOX : public NCNNModel, public ImageModel {
 public:
  HandDetectorYOLOX(const char* param_buffer,
                    const unsigned char* weight_buffer,
                    float score_threshold = 0.7, float iou_threshold = 0.5,
                    int input_width = 416, int input_height = 416,
                    bool use_gpu = false);

  HandDetectorYOLOX(const std::string& param_file,
                    const std::string& weight_file, float score_threshold = 0.7,
                    float iou_threshold = 0.5, int input_width = 416,
                    int input_height = 416, bool use_gpu = false);

  /// Detect faces in an image.
  /// Return 0 on success, otherwise return error code.
  int Predict(const cv::Mat& image,
              std::vector<daisykit::types::Object>& hands);

 private:
  /// Preprocess image data to obtain net input.
  void Preprocess(const cv::Mat& image, ncnn::Mat& net_input) override;

  /// Score threshold for bounding box. Currently we ignore to set this value
  /// because the current model has an internal of 0.25 inside and that's
  /// enough.
  float score_threshold_;

  /// IoU threshold for NMS. Currently we ignore this value because our current
  /// model doesn't need NMS.
  float iou_threshold_;

  /// Cache scale from preprocess step
  /// For restoring on postprocess
  float scale_;
};

}  // namespace models
}  // namespace daisykit

#endif
