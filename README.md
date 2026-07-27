## Live reactive simulation of me and the boids

This repo contains the source code for the project "Me and the boids", an interactive computer simulation very similar to boids.

Notes:
- use instanced rendering, all of them look the same.
- Combine the offsets and the rotations into one model matrix. Possibly create a separate buffer to the quad_vertices, and use glbuffersubdata. check how to do multiple buffer with the shader.
- optical flow from opencv nad camera.

(-1, 1)
(-0.8, 1)
(-0.6, 1)
(-0.4, 1)
(-0.2, 1)
(0., 1)
(0.2, 1)
(0.4, 1)
(0.6, 1)
(0.8, 1)
(-1.0, 0.8)
(-0.8, 0.8)
(-0.6, 0.8)
(-0.4, 0.8)
(-0.2, 0.8)
(0., 0.8)
(0.2, 0.8)
(0.4, 0.8)
(0.6, 0.8)
(0.8, 0.8)
