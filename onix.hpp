#pragma once

#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"
#include <filesystem>
#include <string_view>

namespace Onix {
using OrtFileString = std::basic_string<ORTCHAR_T>;
class OnixInstance {

  OnixInstance() = default;
  OnixInstance(std::string_view &model_path);

private:
  std::string model_name;
  std::string_view log_file{"onnx_runtime.log"};
  Ort::Logger logger;
  auto initialize();
  static OrtFileString toOrtFileString(const std::filesystem::path &path);

  void register_execution_providers(Ort::Env &env);

  Ort::ConstMemoryInfo
  match_common_memory_info(const Ort::Session &input_session,
                           const Ort::Session &output_session);

  Ort::SessionOptions create_session_options(Ort::Env &env, const Opts &opts);

  Ort::Session create_session(Ort::Env &env, std::filesystem::path &model_file,
                              const Ort::SessionOptions &session_options);
  auto load_onnx_model();
};
}; // namespace Onix
