## [1.9.1](https://github.com/Say-Goodbi/Raytracer/compare/v1.9.0...v1.9.1) (2026-05-18)


### Bug Fixes

* livescene thread wait ([6d74d99](https://github.com/Say-Goodbi/Raytracer/commit/6d74d990d3de99e226c92c5f8f6f0f771a48043a))

# [1.9.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.8.0...v1.9.0) (2026-05-18)


### Bug Fixes

* CMake SFML fetch & output directory ([dbfd866](https://github.com/Say-Goodbi/Raytracer/commit/dbfd8663cdba27c72bbf44f384a911db75e55147))


### Features

* add using directive for sf namespace in Livescene.cpp ([ae7c6f3](https://github.com/Say-Goodbi/Raytracer/commit/ae7c6f34945755a623d55036425aceeb369d94b3))
* **build:** remove specific GIT_TAG for SFML dependency in CMakeLists.txt ([261a251](https://github.com/Say-Goodbi/Raytracer/commit/261a2516c73bb322ae8716291565b4f35d833b24))
* **build:** update SFML dependency to use FetchContent with specific version ([3119403](https://github.com/Say-Goodbi/Raytracer/commit/3119403822c771d327377d2a1077fba6c5109404))
* **ci:** add feat/test-release-mirror branch to CI workflow triggers ([a354ba8](https://github.com/Say-Goodbi/Raytracer/commit/a354ba8a4da5c49a7d0407b37d3bc0abe621f54a))
* **ci:** add libudev-dev dependency ([310ef8b](https://github.com/Say-Goodbi/Raytracer/commit/310ef8bec9dab1e3192fb983e2407c202f63c7fd))
* **ci:** add libudev-dev dependency ([b49d9dc](https://github.com/Say-Goodbi/Raytracer/commit/b49d9dc21f12a84e78e41dc811f3e8eca7794e04))
* **ci:** add libudev-dev dependency ([7a14f35](https://github.com/Say-Goodbi/Raytracer/commit/7a14f357fcff7058a5173c5b46110e5e00904fc1))
* **ci:** add libudev-dev dependency for Doxygen installation ([be0e5fb](https://github.com/Say-Goodbi/Raytracer/commit/be0e5fbd5ec25bf488c57eade692c070da514f6a))
* **ci:** add libudev-dev dependency for Doxygen installation ([c3d52de](https://github.com/Say-Goodbi/Raytracer/commit/c3d52de2af2564ba95ec6b2562469019c80c0372))
* **ci:** update CI dependencies to include libcriterion-dev ([cf90988](https://github.com/Say-Goodbi/Raytracer/commit/cf90988828825fefe91f9500a5b92993884558a5))
* **ci:** update dependency installation step to include package update ([52794c7](https://github.com/Say-Goodbi/Raytracer/commit/52794c7d4128830356b449da72b835df3c83baef))
* refactor Livescene rendering and event handling for improved clarity and performance ([f344203](https://github.com/Say-Goodbi/Raytracer/commit/f344203f4fa21e2c06eb596a38cfc9b57625ffac))
* refactor material instance retrieval to use a lambda function ([014141c](https://github.com/Say-Goodbi/Raytracer/commit/014141cc25d859dcc0bae045a4f098e024d7cd99))
* **tests:** add comprehensive parsing tests and improve error handling in Parser ([1bcf73a](https://github.com/Say-Goodbi/Raytracer/commit/1bcf73a2ab4b4b8a7a9fe5184643efad400d905c))
* **tests:** add unit testing framework and initial test case ([66a5e75](https://github.com/Say-Goodbi/Raytracer/commit/66a5e7575fcc66ad1029c4dc75f3c21ecd8e3404))
* **tests:** enhance unit test setup to include additional sources and independent test registration ([329db20](https://github.com/Say-Goodbi/Raytracer/commit/329db202fc381d2bed09eff8b9e37b22dbc4ad53))
* **tests:** improve test registration and coverage options for unit tests ([1ed3398](https://github.com/Say-Goodbi/Raytracer/commit/1ed33988f9372a3baaaceb2b276c2fd16bacab45))
* **tests:** remove test case for non-existent configuration file ([d625fc7](https://github.com/Say-Goodbi/Raytracer/commit/d625fc71e8e0ea83add4e04962f6d6cf2870b8b1))

# [1.8.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.7.0...v1.8.0) (2026-05-17)


### Features

* cone primitive ([a1543de](https://github.com/Say-Goodbi/Raytracer/commit/a1543deff08f5a5faec03764a841bb6a27b87db5))

# [1.7.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.6.1...v1.7.0) (2026-05-17)


### Features

* implement BVH camera renderer and related configuration updates, and delete useBVH from Camera ([c56d176](https://github.com/Say-Goodbi/Raytracer/commit/c56d1760c51c26e8ffda75f4aa41b5b51789ace6))

## [1.6.1](https://github.com/Say-Goodbi/Raytracer/compare/v1.6.0...v1.6.1) (2026-05-17)


### Bug Fixes

* scale on camera transform matrix now functional ([c634034](https://github.com/Say-Goodbi/Raytracer/commit/c634034832baaf11f35261ffb864735b07a1e4b6))

# [1.6.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.5.0...v1.6.0) (2026-05-16)


### Bug Fixes

* cylinder height + scenes calibration ([4756b48](https://github.com/Say-Goodbi/Raytracer/commit/4756b484c358d430b20a2bf96b168620526d5687))


### Features

* add Livescene plugin for real-time scene rendering with SFML integration. ([f998316](https://github.com/Say-Goodbi/Raytracer/commit/f998316184c4f0db193866e14da04d63a3715eff))
* implement framebuffer mutex for thread-safe rendering and add unloadPlugins method in PluginManager ([c9f561d](https://github.com/Say-Goodbi/Raytracer/commit/c9f561d77d3af0b660d36a566e3d19bd8277dbcb))
* new complete documentation ([16548a7](https://github.com/Say-Goodbi/Raytracer/commit/16548a7e730e9bc40ffec5484a8acfbdb40c7552))
* new scene ([68d1819](https://github.com/Say-Goodbi/Raytracer/commit/68d181913245b3c03a022c3332582217b3d007fc))

# [1.5.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.4.0...v1.5.0) (2026-05-16)


### Bug Fixes

* another iteration of glass ([de5ff8c](https://github.com/Say-Goodbi/Raytracer/commit/de5ff8c81f28a4ae89f3f76bf4d621272724678e))


### Features

* enhance SceneWriter and SceneLoader to support dynamic parameters and improve interface handling, allowing modular interfaces ([a7533cb](https://github.com/Say-Goodbi/Raytracer/commit/a7533cbace35ac0805ab5da0f01069cdf3b5dff8))
* glass material ([2135a1e](https://github.com/Say-Goodbi/Raytracer/commit/2135a1eed921eff7cfda9448905ab2d04f554ee7))

# [1.4.0](https://github.com/Say-Goodbi/Raytracer/compare/v1.3.1...v1.4.0) (2026-05-14)


### Bug Fixes

* camera matrix initialization maths and config files rotations ([f89b7e3](https://github.com/Say-Goodbi/Raytracer/commit/f89b7e3ff17c657a4ee2b70e90754f37c6f07f0e))
* missing default key components throws an error instead of crashing ([19d2964](https://github.com/Say-Goodbi/Raytracer/commit/19d29647a336ef600a76d05875502a0046104ac5))


### Features

* acceleration data structure inside scenes, using bounding volume hierarchy (bvh) built on surface area heuristic (sah) ([da2829e](https://github.com/Say-Goodbi/Raytracer/commit/da2829ee2fe1af8954b49828dc8e1818fc3d2265))
* primitives holding bounding boxes method aabb ([42f883c](https://github.com/Say-Goodbi/Raytracer/commit/42f883c557e2d2ed67c3993bcd0b409c27f3d153))

## [1.3.1](https://github.com/Say-Goodbi/Raytracer/compare/v1.3.0...v1.3.1) (2026-05-14)


### Bug Fixes

* update camera configurations and add parsing exception handling, with component loader helpers ([b7f776e](https://github.com/Say-Goodbi/Raytracer/commit/b7f776e0510941c5c7f83f4dff98763201f0bba6))

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
