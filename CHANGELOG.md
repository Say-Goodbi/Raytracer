# [1.3.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.2.1...v1.3.0) (2026-05-14)


### Bug Fixes

* matrix protected value access ([962e8e2](https://github.com/Say-Goodbi/Raytracer/commit/962e8e2d39501ed82ff3f08ea28f1d96b64dee93))


### Features

* add cylinder primitive support in CorePlugin ([a6fda0c](https://github.com/Say-Goodbi/Raytracer/commit/a6fda0c4ee1cd14a1841aecc7bcd239737b9646a))
* add DivisionByZeroException and Matrix class implementation ([b19db2d](https://github.com/Say-Goodbi/Raytracer/commit/b19db2d0c24a0f94e694ce4b4abca302af13f0e3))
* add DivisionByZeroException and Matrix class implementation ([77dc3d3](https://github.com/Say-Goodbi/Raytracer/commit/77dc3d3694c75549a49614e866e53b0ef3aeaebc))
* camera now uses 4x4 affine transformation matrix ([8d40543](https://github.com/Say-Goodbi/Raytracer/commit/8d40543c060d7a0836d44766828b62fe894dc5a0))
* implement material handling and support for lambertian material in CorePlugin ([0548ad9](https://github.com/Say-Goodbi/Raytracer/commit/0548ad939f0b59603970ff21ecc8eed0f81b5ca3))
* phong material + fix ([add9195](https://github.com/Say-Goodbi/Raytracer/commit/add919524a0b5a830dfc8fb309df728504110db6))

## [1.2.1](https://github.com/Say-Goodbi/Raytracer/compare/v1.2.0...v1.2.1) (2026-05-12)


### Bug Fixes

* cmake ([4f5ef2b](https://github.com/Say-Goodbi/Raytracer/commit/4f5ef2b534842fad8e06753c590d1ecc8ba04af0))

# [1.2.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.1.0...v1.2.0) (2026-05-12)


### Bug Fixes

* compile errors reducted to SceneLoader, which needs heavy rework ([d0f91f9](https://github.com/Say-Goodbi/Raytracer/commit/d0f91f9976519a135e1239137d880c809e379b4d))
* package management ([55eb931](https://github.com/Say-Goodbi/Raytracer/commit/55eb9319435dcede835e9925b179b856dfaa8a38))
* update include paths and improve type safety in color computation ([4515900](https://github.com/Say-Goodbi/Raytracer/commit/4515900abe208b99c10e391e4d378a3422c31948))
* updated the rendenring logic ([1bd4291](https://github.com/Say-Goodbi/Raytracer/commit/1bd4291c87df729ecf4f0226319d92c3736c28f6))


### Features

* cmakelist ([be9cc6f](https://github.com/Say-Goodbi/Raytracer/commit/be9cc6f6800ef7ab801439e9923dcd1cd2bd1efa))
* cylinder primitive ([aad303c](https://github.com/Say-Goodbi/Raytracer/commit/aad303c59fcbacc3037db919607f878367d1ff97))
* implement Monte Carlo path tracing with PBR material interface ([428f2cf](https://github.com/Say-Goodbi/Raytracer/commit/428f2cfdc520524b1d663d027bd0ea1760f4e886))
* **light:** add ambient light implementation ([8393035](https://github.com/Say-Goodbi/Raytracer/commit/8393035c3432f48a0086342746db54df5ffcb301))
* **light:** add directional light implementation ([7a2a710](https://github.com/Say-Goodbi/Raytracer/commit/7a2a71052e84c00c5c3785ba96fb0cf0d27bb460))
* **light:** add point light implementation ([a7ae251](https://github.com/Say-Goodbi/Raytracer/commit/a7ae2518c28fcc803c9ba044466fc35449af5c3d))
* **light:** incorporate direct light calculation in ray casting ([f10f7b7](https://github.com/Say-Goodbi/Raytracer/commit/f10f7b7b694b938449b9db474eec8d787c7ea41f))
* main ([901edd8](https://github.com/Say-Goodbi/Raytracer/commit/901edd80157d2c541e32952d7aa524d18ff34e8f))
* **parser:** add support for vector objects in configuration parsing ([5779baf](https://github.com/Say-Goodbi/Raytracer/commit/5779baf54c7eeaf4b55b7828697fd72c10a93905))
* **parser:** implement configuration parser class for reading and parsing settings ([e33d25a](https://github.com/Say-Goodbi/Raytracer/commit/e33d25ad1c7f559f4568c1b70506ee2e48c87419))
* replace FlatColor with Lambertian BRDF and fix Plane hit ([1081413](https://github.com/Say-Goodbi/Raytracer/commit/108141380b3175c81fad3a12825cf66cc7a432fd))
* **scene:** implement scene and scene loader with camera and light configurations ([1975de5](https://github.com/Say-Goodbi/Raytracer/commit/1975de57fe65798dbf503e9b21d85113c93e2752))
* sphere primitive ([ded6db6](https://github.com/Say-Goodbi/Raytracer/commit/ded6db6b1a32b304fa95a25d2a3c6c3366088618))
* sphere primitive ([f6a4f97](https://github.com/Say-Goodbi/Raytracer/commit/f6a4f9752f2bbc0bfdd4c5605807c756df442e3b))
* utils random generator ([5401306](https://github.com/Say-Goodbi/Raytracer/commit/5401306ae5a6fd30168c3773794fa926e9fbac1c))

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
