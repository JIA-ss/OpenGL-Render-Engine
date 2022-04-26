#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <vector>

class RayTracing
{
public:
    void Update();
};

class Ray;
struct Hit_Record;
class Hittable;
class Sphere;
class Hittable_List;
class material;
class lambertian;
class metal;
class dielectric;
class camera;
#include "RayTracing.inl"