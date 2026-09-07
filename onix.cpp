#include "onix.hpp"
#include <iostream>

namespace Onix {
auto OnixInstance::initialize() {
  auto env =
      Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, model_name.c_str());
  env.RegisterExecutionProviderLibrary(
      "openvino", ORT_TSTR("onnxruntime_providers_openvino.dll"));
  env.RegisterExecutionProviderLibrary(
      "qnn", ORT_TSTR("onnxruntime_providers_qnn.dll"));
  env.RegisterExecutionProviderLibrary(
      "nv_tensorrt_rtx", ORT_TSTR("onnxruntime_providers_nv_tensorrt_rtx.dll"));
  auto ep_devices = env.GetEpDevices();
  auto selected_devices = my_ep_selection_function(ep_devices);

  Ort::SessionOptions session_options;
  session_options.AppendExecutionProvider_V2(env, selected_devices, ep_options);
  // Optionally, set device policy. E.g.
  // OrtExecutionProviderDevicePolicy_PREFER_GPU,
  // OrtExecutionProviderDevicePolicy_PREFER_NPU,
  // OrtExecutionProviderDevicePolicy_MAX_PERFORMANCE
  session_options.SetEpSelectionPolicy(
      OrtExecutionProviderDevicePolicy_PREFER_GPU);
}

OrtFileString OnixInstance::toOrtFileString(const std::filesystem::path &path) {
  std::string string(path.string());
  return {string.begin(), string.end()};
}
void OnixInstance::register_execution_providers(Ort::Env &env) {
  // clang-format off

  std::array provider_libraries{
    std::pair{"nv_tensorrt_rtx","libonnxruntime_providers_nv_tensorrt_rtx.so"},
    std::pair{"cuda","libonnxruntime_providers_cuda.so"},
    std::pair{"openvino","libonnxruntime_providers_openvino.so"},
  };

  for (auto &[registration_name, dll] : provider_libraries) {
    auto providers_library = get_executable_path().parent_path() / dll;
    if (!std::filesystem::is_regular_file(providers_library)) {
      logger.LogMessage(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, log_file, 0,
                        "register_execution_providers",
                        "{} does not exist! Skipping execution provider",
                        providers_library.string());
      continue;
    }
    try {
      env.RegisterExecutionProviderLibrary(registration_name,
                                           toOrtFileString(providers_library));
    } catch (std::exception &ex) {
      logger.LogMessage(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, log_file, 0,
                        "register_execution_providers",
                        "Failed to register {}! Skipping execution provider",
                        providers_library.string());
    }
  }
}

Ort::ConstMemoryInfo
OnixInstance::match_common_memory_info(const Ort::Session &input_session,
                                       const Ort::Session &output_session) {
  auto input_infos = input_session.GetMemoryInfoForOutputs();
  auto output_infos = output_session.GetMemoryInfoForInputs();

  // First try to find a common non-CPU allocator
  for (auto &in : input_infos) {
    for (auto &out : output_infos) {
      if (in == out && in.GetDeviceType() != OrtMemoryInfoDeviceType_CPU &&
          in.GetDeviceMemoryType() == OrtDeviceMemoryType_DEFAULT) {
        return in;
      }
    }
  }
  // If impossible then also allow to fall back to CPU
  for (auto &in : input_infos) {
    for (auto &out : output_infos) {
      if (in == out) {
        return in;
      }
    }
  }
  logger.LogMessage(OrtLoggingLevel::ORT_LOGGING_LEVEL_ERROR, log_file, 0,
                    "register_execution_providers",
                    "Could not find a common allocator");
  std::cerr << "Could not find a common allocator" << std::endl;
}

Ort::SessionOptions OnixInstance::create_session_options(Ort::Env &env,
                                                         const Opts &opts) {
  std::vector<Ort::ConstEpDevice> selected_devices;
  auto ep_devices = env.GetEpDevices();
  logger.LogFormattedMessage(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO, log_file,
                             0, "create_session_options", "{} devices found",
                             ep_devices.size());
  for (auto &device : ep_devices) {
    auto metadata = device.Device().Metadata();
    // LUID can be used on Windows platform to match EpDevices with
    // IDXGIAdapter in case an application already has a device selection
    // logic based on `IDXGIAdapter`s
    auto luid = metadata.GetValue("LUID");
    logger.LogFormattedMessage(
        OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO, log_file, 0,
        "create_session_options",
        "Vendor: {}, EpName: {}, DeviceId: 0x{:x}, LUID: {}", device.EpVendor(),
        device.EpName(), device.Device().DeviceId(),
        luid ? luid : "<unavailable>");
    if (to_uppercase(opts.select_vendor) == device.Device().Vendor()) {
      selected_devices.push_back(device);
    }
    if (to_uppercase(opts.select_ep) == device.EpName()) {
      selected_devices.push_back(device);
    }
  }

  Ort::SessionOptions so;
  if (!selected_devices.empty()) {
    Ort::KeyValuePairs ep_options;
    // Select EP for manually selected devices
    so.AppendExecutionProvider_V2(env, selected_devices, ep_options);
  }

  so.SetEpSelectionPolicy(opts.ep_device_policy);
  return so;
}

Ort::Session
OnixInstance::create_session(Ort::Env &env, std::filesystem::path &model_file,
                             const Ort::SessionOptions &session_options) {
  if (!std::filesystem::is_regular_file(model_file)) {
    logger.LogFormattedMessage(
        OrtLoggingLevel::ORT_LOGGING_LEVEL_ERROR, log_file, 0, "create_session",
        "Model file {} does not exist!", model_file.string());
    std::cerr << "Model file " << model_file.string() << " does not exist!"
              << std::endl;
  }

  Ort::Session session(env, toOrtFileString(model_file).c_str(),
                       session_options);
  return session;
}
auto OnixInstance::load_onnx_model() {}
}// namespace Onix
