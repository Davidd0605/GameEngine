# Kartof Engine
> A lightweight C++ OpenGL game engine built around an entity-component architecture.

![Engine Screenshot](https://github.com/user-attachments/assets/cb7fec3b-5ddb-4263-bbef-0d1a07674328)
<img width="1504" height="834" alt="image" src="https://github.com/user-attachments/assets/1d46b1fd-4d2b-464b-8712-867735f48a59" />

---

## Features

### Entity Component System
- `gameObject` class acts as a pure container — no logic of its own
- **Components** for engine-managed behaviour (Transform, Camera, Light, Mesh)
- **Functionalities** for user-defined behaviour (extend `Functionality` to script anything)
- Runtime component lookup via `getComponent<T>()` with no external dependencies

### Rendering
- OpenGL 4.3 core profile
- Phong lighting model with support for **directional** and **point** lights
- **Post-processing pipeline** — stack multiple shader passes on top of each other
  - Volumetric fog (blends with scene light color)
  - Bloom
  - Edge detection
  - Pixelation
  - Painting effect
- Framebuffer-based camera rendering with optional post-processing per camera
- Per-camera priority system — control render order across multiple cameras

### Model Loading
- `.gltf` model loading via a custom `ModelLoader`
- Loaded models integrate directly into the ECS as `gameObject` hierarchies with Transform parenting
- Material system with per-mesh shader and texture binding

### Scene System
- `GameScene` manages game objects and systems
- Systems (e.g. `RenderSystem`) are decoupled from the scene and operate on it externally
- **Scene serialization** — saves the full scene to JSON on exit (`scenes/<name>.json`)
- **Scene deserialization** via `SceneSerializer` — reloads a saved scene from file with a single call, reconstructing all objects, components, lights, cameras, and post-processing passes

---

## Usage

Toggle between manual scene setup and loading from a saved file with a single flag in `main.cpp`:

```cpp
bool loadFromFile = false; // flip to true to load from scenes/
```

---

## Project Structure
