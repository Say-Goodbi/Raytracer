# Config Parser 

This directory contains the implementation of the `Parser` class, which is responsible for reading and parsing configuration files for the ray tracer. The parser uses the `libconfig` library to read configuration files in a structured format, and it converts the settings into a format that can be easily accessed by other parts of the ray tracer.

## Description of the Nodes

The configuration settings are stored in a tree-like structure, where each node can be a scalar value (such as an integer, float, string, or boolean), an object (a collection of key-value pairs), or a vector of objects (a list of objects). The `Node` struct is defined to represent this structure, and it uses `std::variant` to allow for different types of values.

```cpp
struct Node;
typedef std::variant<int, long long, float, std::string, bool> ScalarValue;
typedef std::unique_ptr<Node> NodePtr;
typedef std::map<std::string, NodePtr> Object;
typedef std::vector<Object> VectorObject;

struct Node
{
    std::variant<ScalarValue, Object, VectorObject> value;
};
```

## Usage

To use the `Parser` class, you can create an instance of the class by providing the filename of the configuration file you want to read. The parser will read the file and build the configuration tree. You can then access the settings using the _node member of the `Parser` class, which contains the root of the configuration tree.

- Example usage:
```cpp
try {
    RayTracer::Parser parser("config.cfg");
      const RayTracer::Object &rootObject = std::get<RayTracer::Object>(parser._node->value);
      const RayTracer::Node &cameraNode = *rootObject.at("camera");
      const RayTracer::Object &cameraObject = std::get<RayTracer::Object>(cameraNode.value);
      const RayTracer::Node &fieldOfViewNode = *cameraObject.at("fieldOfView");
      const RayTracer::ScalarValue &fieldOfViewValue = std::get<RayTracer::ScalarValue>(fieldOfViewNode.value);
      std::cout << "Camera field of view: " << std::get<float>(fieldOfViewValue) << " degrees" << std::endl;
} catch (const RayTracer::RaytracerException &e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

- Example configuration file (config.cfg):
```cfg
# Configuration of the camera
camera:
{
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = -100; z = 20; };
    rotation = { x = 0; y = 0; z = 0; };
    fieldOfView = 72.0; # In degree
};

# Primitives in the scene
primitives:
{
    # List of spheres
    spheres = (
        { x = 60; y = 5; z = 40; r = 25; color = { r = 255; g = 64; b = 64; }; },
        { x = -40; y = 20; z = -10; r = 35; color = { r = 64; g = 255; b = 64; }; }
    );

    # List of planes
    planes = (
        { axis = "Z"; position = -20; color = { r = 64; g = 64; b = 255; }; }
    );
};

# Light configuration
lights:
{
    ambient = 0.4; # Multiplier of ambient light
    diffuse = 0.6; # Multiplier of diffuse light

    # List of point lights
    point = (
        { x = 400; y = 100; z = 500; }
    );

    # List of directional lights
    directional = ();
};
```

- Example of data structure after parsing:
```cpp
{
  camera = {
    fieldOfView = 72
    position = {
      x = 0
      y = -100
      z = 20
    }
    resolution = {
      height = 1080
      width = 1920
    }
    rotation = {
      x = 0
      y = 0
      z = 0
    }
  }
  lights = {
    ambient = 0.4
    diffuse = 0.6
    point = [
      {
        x = 400
        y = 100
        z = 500
      }
    ]
  }
  primitives = {
    planes = [
      {
        axis = Z
        color = {
          b = 255
          g = 64
          r = 64
        }
        position = -20
      }
    ]
    spheres = [
      {
        color = {
          b = 64
          g = 64
          r = 255
        }
        r = 25
        x = 60
        y = 5
        z = 40
      }
      {
        color = {
          b = 64
          g = 255
          r = 64
        }
        r = 35
        x = -40
        y = 20
        z = -10
      }
    ]
  }
}
```