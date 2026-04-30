# Raytracer

A modular, plugin-oriented raytracing engine built in C++. Separates concerns across geometry, primitives, materials, and rendering, allowing extensibility through plugins.

## Documentation

For comprehensive architecture documentation, design rationale, and API reference, see the [full documentation](https://say-goodbi.github.io/Raytracer/index.html).

## Overview

This project implements a clean, extensible raytracing framework with:
- **Plugin architecture** for custom renderers, primitives, materials, and lights
- **Separation of concerns** — rendering logic is independent from I/O
- **Modular design** — easy to extend and customize without modifying core code

## Quick Start


## Architecture

- **ARenderer** / **Camera** — rendering abstraction and concrete camera implementation
- **AInterface** / **SceneWriter** — application interface and concrete file output
- **Scene** — container for camera, primitives, and lights
- **Geometry** — Point3D, Vector3D, Ray, Rectangle3D, HitRecord
- **Primitives & Materials** — pluggable shapes and shaders

See the full documentation for details on extending with custom plugins.