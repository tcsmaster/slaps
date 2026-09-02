## Live reactive simulation of me and the boids

This repo contains the source code for the project "Me and the boids", an interactive computer simulation very similar to boids.

Notes:
- base vector field (probably curl of random noise)
  - From video, the particles that are moved via input motion are destroyed (possibly turned around back to the stream).
- The length of opencv vector cant just be acceleration, because it needs to change the velocity vector. But it should use the vector as the new velocity. Use slerp to move the velocity vector to the input motion vector. Use the length of the opencv vector as acceleration.
- should there even be a looping-around mechanism? The og just let it go out of bounds. Probably destroy if it is no longer visible (track midpoint and check if any corner is still visible ('casue rectangle is convex)).
- make the borders stick to the window sides (glfwwindowresize callback).
- all the different pose estimations (blazepose, openpose)
- map the skeletal position to the view space

Tried out optical flow algorithms, sparse vs dense, it works really good for motion, but whenever there is no motion, noise comes in (clipping works well). However, it would move all sprites with my body, instead I just want a few sprites to move. Pivot to skeletal tracking

References:
- [curl noise vector field](https://emildziewanowski.com/curl-noise/)
