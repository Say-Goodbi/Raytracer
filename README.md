# Raytracer

A modular, plugin-oriented raytracing engine built in C++. Separates concerns across geometry, primitives, materials, and rendering, allowing extensibility through plugins.

## Documentation

This project implements a clean, extensible raytracing framework with:
- **Plugin architecture** for custom output interfaces, renderers, primitives, materials, and lights
- **Separation of concerns** — rendering logic is independent from I/O
- **Modular design** — easy to extend and customize without modifying core code


For comprehensive architecture documentation, design rationale, and API reference, see the [full documentation](https://say-goodbi.github.io/Raytracer/index.html).

## Quick Start

#### Clone the repository

```bash
git clone git@github.com:Say-Goodbi/Raytracer.git Raytracer
cd Raytracer
```

#### Build the executable (requires `cmake`)

On Linux :
```
mkdir build && cmake -S. -Bbuild && make -Cbuild
```

#### Run it

```bash
./raytracer <scene_file>
```
Scene files are disponible in the `config` directory.
This will write a `output.ppm` file in the current directory.

You can also experiment other interfaces and flags.
```bash
./raytracer <scene_config_file> [-i <interface> [flags]]
```

## Architecture

- **ARenderer** / **Camera** — rendering abstraction and concrete camera implementation
- **AInterface** / **SceneWriter** — application interface and concrete file output
- **Scene** — container for camera, primitives, and lights
- **Geometry** — Point3D, Vector3D, Ray, Rectangle3D, HitRecord
- **Primitives & Materials** — pluggable shapes and shaders

See the full documentation for details on extending with custom plugins, or create your own scene configuration.