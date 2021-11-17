#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <string>

#include "daisykitsdk/common/types.h"
#include "daisykitsdk/flows/background_matting_flow.h"
#include "daisykitsdk/flows/barcode_scanner_flow.h"
#include "daisykitsdk/flows/face_detector_flow.h"
#include "daisykitsdk/flows/human_pose_movenet_flow.h"
#include "ndarray_converter.h"

using namespace daisykit;
namespace py = pybind11;

PYBIND11_MODULE(daisykit, m) {
  NDArrayConverter::init_numpy();

  py::class_<types::Face>(m, "Face")
      .def_readwrite("x", &types::Face::x)
      .def_readwrite("y", &types::Face::y)
      .def_readwrite("w", &types::Face::w)
      .def_readwrite("h", &types::Face::h)
      .def_readwrite("confidence", &types::Face::confidence)
      .def_readwrite("wearing_mask_prob", &types::Face::wearing_mask_prob)
      .def_readwrite("landmark", &types::Face::landmark);

  py::class_<types::Keypoint>(m, "Keypoint")
      .def_readwrite("x", &types::Keypoint::x)
      .def_readwrite("y", &types::Keypoint::y)
      .def_readwrite("confidence", &types::Keypoint::confidence);

  py::class_<types::Box>(m, "Box")
      .def(py::init<>())
      .def(py::init<float, float, float, float>(), py::arg("x"), py::arg("y"),
           py::arg("w"), py::arg("h"))
      .def_readwrite("x", &types::Box::x)
      .def_readwrite("y", &types::Box::y)
      .def_readwrite("w", &types::Box::w)
      .def_readwrite("h", &types::Box::h);

  py::class_<types::Object>(m, "Object")
      .def(py::init<>())
      .def(py::init<float, float, float, float, int, float>(), py::arg("x"),
           py::arg("y"), py::arg("w"), py::arg("h"), py::arg("class_id"),
           py::arg("confidence"))
      .def_readwrite("x", &types::Object::x)
      .def_readwrite("y", &types::Object::y)
      .def_readwrite("w", &types::Object::w)
      .def_readwrite("h", &types::Object::h)
      .def_readwrite("class_id", &types::Object::class_id)
      .def_readwrite("confidence", &types::Object::confidence);

  py::class_<ObjectWithKeypoints>(m, "HumanPose")
      .def(py::init<>())
      .def(
          py::init<const types::Object&, const std::vector<types::Keypoint>&>(),
          py::arg("body"), py::arg("keypoints"))
      .def_readwrite("x", &ObjectWithKeypoints::x)
      .def_readwrite("y", &ObjectWithKeypoints::y)
      .def_readwrite("w", &ObjectWithKeypoints::w)
      .def_readwrite("h", &ObjectWithKeypoints::h)
      .def_readwrite("class_id", &ObjectWithKeypoints::class_id)
      .def_readwrite("confidence", &ObjectWithKeypoints::confidence)
      .def_readwrite("keypoints", &ObjectWithKeypoints::keypoints);

  py::class_<flows::FaceDetectorFlow>(m, "FaceDetectorFlow")
      .def(py::init<const std::string&>(), py::arg("config_path"))
      .def("Process", &flows::FaceDetectorFlow::Process)
      .def("DrawResult", &flows::FaceDetectorFlow::DrawResult, py::arg("image"),
           py::arg("faces"), py::return_value_policy::reference_internal);

  py::class_<flows::BackgroundMattingFlow>(m, "BackgroundMattingFlow")
      .def(py::init<const std::string&, const cv::Mat&>(),
           py::arg("config_path"), py::arg("default_background"))
      .def("Process", &flows::BackgroundMattingFlow::Process)
      .def("DrawResult", &flows::BackgroundMattingFlow::DrawResult,
           py::return_value_policy::reference_internal);

  py::class_<flows::BarcodeScannerFlow>(m, "BarcodeScannerFlow")
      .def(py::init<const std::string&>(), py::arg("config_path"))
      .def("Process", &flows::BarcodeScannerFlow::Process, py::arg("image"),
           py::arg("draw"));

  py::class_<flows::HumanPoseMoveNetFlow>(m, "HumanPoseMoveNetFlow")
      .def(py::init<const std::string&>(), py::arg("config_path"))
      .def("Process", &flows::HumanPoseMoveNetFlow::Process, py::arg("image"))
      .def("DrawResult", &flows::HumanPoseMoveNetFlow::DrawResult,
           py::arg("image"), py::arg("poses"),
           py::return_value_policy::reference_internal);

  m.doc() = R"pbdoc(
        DaisyKit python wrapper
        -----------------------
        .. currentmodule:: pydaisykit
        .. autosummary::
           :toctree: _generate
    )pbdoc";

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
