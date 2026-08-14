## Live reactive simulation of me and the boids

This repo contains the source code for the project "Me and the boids", an interactive computer simulation very similar to boids.

Notes:
- optical flow from opencv and camera. Try out different dense flows.
- Need to think about the transition from automatic motion to movement-accelerated motion, then back to automatic.
- The length of opencv vector cant just be acceleration, because it needs to change the velocity vector. But it should use the vector as the new velocity. Maxbe interpolate between the current and the opencv vector. Use the length of the opencv vector as acceleration.
- should there even be a looping-around mechanism? The og just let it go out of bounds.
- make the borders stick to the window sides
