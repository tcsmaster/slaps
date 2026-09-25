## Live reactive simulation of me and the boids

This repo contains the source code for the project "Me and the boids", an interactive computer simulation very similar to boids.

Notes:
  - From video, the particles that are moved via input motion are destroyed (possibly turned around back to the stream).
- The length of opencv vector cant just be acceleration, because it needs to change the velocity vector. But it should use the vector as the new velocity. Use slerp to move the velocity vector to the input motion vector. Use the length of the opencv vector as acceleration.
- should there even be a looping-around mechanism? The og just let it go out of bounds. Probably destroy if it is no longer visible (track midpoint and check if any corner is still visible ('casue rectangle is convex)).
- make the borders stick to the window sides (glfwwindowresize callback).
- all the different pose estimations (blazepose, openpose)
- map the skeletal position to the view space
- hand estimation instead of pose estimation for potential speedup vs probably some accuracy loss
- does onnx run bettero on cpu or gpu, start with cpu compilation.

## Whole ass pipeline
- Camera: Captures a frame and stores it in a cv Mat variable
- onnx model runs pose/hand estimation on the frame, stores the results (17 to 33 3d coordinates) somewhere, on the cpu for now.
- In parallel to this, the flow is going on.
- TODO: finish the pipeline desc.
## Shaders:
Compute shader (every frame):
   read tracked skeleton uniform/buffer → influences field
   evaluate curl noise + tracking-based attractor at each particle's position
   integrate velocity/position, write compact state to SSBO
        ↓ (stays on GPU, zero CPU involvement)
Vertex shader (instanced draw):
   read compact state (pos, angle, scale) from same SSBO
   reconstruct minimal 2x2 rotation, transform quad corners
   output clip-space position
        ↓
Fragment shader:
   texture/brushstroke sprite shading
## Main pipeline
OpenCV webcam
      ↓
cv::Mat
      ↓
OpenVINO GPU
      ↓
Pose
      ↓
OpenGL SSBO
      ↓
Render

## Video notes
Tried out optical flow algorithms, sparse vs dense, it works really good for motion, but whenever there is no motion, noise comes in (clipping works well). However, it would move all sprites with my body, instead I just want a few sprites to move. Pivot to skeletal tracking. Needs onnxruntime. With my iGPU, I need to build the appropriate version of onnxruntime using openvino. ONNX does/needs hardware interface to run the models more efficiently, called Execution Providers (EP). For Intel cpu, gpu, there is oneDNN and OpenVino. I use the latter.
## Code structure
main.cpp	Program entry point
Application.*	Starts/stops threads and coordinates the application
Camera.*	Captures frames from the built-in webcam
PoseEstimator.*	Loads ONNX through OpenVINO and performs inference
Renderer.*	OpenGL initialization and rendering

## References:
Robert BRidson - Curl-Noise for Procedural Fluid Flow
- [curl noise vector field](https://emildziewanowski.com/curl-noise/)
- [noise shader](https://github.com/ashima/webgl-noise)
