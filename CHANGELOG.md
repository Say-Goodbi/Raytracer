# [1.1.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.0.0...v1.1.0) (2026-04-30)


### Features

* **exception:** add RaytracerException with source_location support ([e3a89d6](https://github.com/Say-Goodbi/Raytracer/commit/e3a89d6365af2a255e17d7fde6d9be25bc710dc8))
* **geometry:** add Ray::at() point-on-ray computation ([f2ee700](https://github.com/Say-Goodbi/Raytracer/commit/f2ee700257f17c5f3067f05cad7d0006816f6014))
* **geometry:** add Vector3D instance dot product and Rectangle3D ([3b9fa9b](https://github.com/Say-Goodbi/Raytracer/commit/3b9fa9b0819744f02d3bdd073ff0d2160b5c5e53))
* **geometry:** wire IMaterial pointer into HitRecord ([557347b](https://github.com/Say-Goodbi/Raytracer/commit/557347b3dcd40b504d5661b25a21ab6837dcb4f7))
* **objects:** define IPrimitive, IMaterial and ILight interfaces ([713c4c3](https://github.com/Say-Goodbi/Raytracer/commit/713c4c37a153cbaba3a9798f3d3f70bbb1ddfac2))
* **objects:** implement APrimitive base class and refactor AMaterial ([265b4d7](https://github.com/Say-Goodbi/Raytracer/commit/265b4d75863974a0f706cb08a05e9a5f6724234c))
* **primitives:** implement Plane hit detection ([02d9461](https://github.com/Say-Goodbi/Raytracer/commit/02d9461847579090844b59246a239b11e48186df))
* **renderer:** implement ray casting with shadow computation ([ca3893e](https://github.com/Say-Goodbi/Raytracer/commit/ca3893ee809ed43fdca21600ec971eac96455b60))
* **scene:** implement Camera ray generation and Scene management ([f8aa1f9](https://github.com/Say-Goodbi/Raytracer/commit/f8aa1f924bf74e4811301eec12459acf0674eabc))
* **utils:** add Color struct with arithmetic operators and clamp ([1ae304d](https://github.com/Say-Goodbi/Raytracer/commit/1ae304d0e0f830cc70b81c2c47e2d3aa51784ad6))

# 1.0.0 (2026-04-28)


### Features

* architecture of the project ([47e04d4](https://github.com/Say-Goodbi/Raytracer/commit/47e04d4ce4827ef403324f4dd3eb5d0e4c8fff56))
