#pragma once
#include "RayTracing.h"
#include <sstream>

class Ray
{
private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
public:
    Ray() = default;
    Ray(const glm::vec3& ori, const glm::vec3& dir)
        : m_origin(ori), m_direction( glm::normalize(dir))
    { }
    glm::vec3 origin() const { return m_origin; }
    glm::vec3 direction() const { return m_direction; }
    glm::vec3 at(float t) const 
    {
        return m_origin + m_direction * t;
    }
};

struct Hit_Record 
{
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
    material* mat;
};


class Hittable 
{
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const = 0;
};


class Sphere : public Hittable 
{
public:
    Sphere() {}
    Sphere(const glm::vec3& cen, float r, material* mat) : center(cen), radius(r), mat(mat) {};

    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

public:
    glm::vec3 center;
    float radius;
    material* mat;
};


class Hittable_List : public Hittable
{
public:
    Hittable_List() {}
    Hittable_List(Hittable* object) { add(object); }

    void clear() { objects.clear(); }
    void add(Hittable* object) { objects.push_back(object); }

    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

    public:
        std::vector<Hittable*> objects;
};


class material
{
public:
    virtual bool scatter
        (
            const Ray& r_in, const Hit_Record& rec, glm::vec3& attenuation, Ray& scattered
        ) const = 0;
    virtual std::string name() = 0;
};

#define DECALRE_SCATTER_FUNCTION(_CLASS_)    \
public: \
    virtual bool scatter(   \
        const Ray& r_in, const Hit_Record& rec, glm::vec3& attenuation, Ray& scattered  \
    ) const override ;  \
    std::string name() override { return #_CLASS_; }

class lambertian : public material 
{
    DECALRE_SCATTER_FUNCTION(lambertian)
public:
    lambertian(const glm::vec3& a) : albedo(a) {}
public:
    glm::vec3 albedo;
};

class metal : public material 
{
    DECALRE_SCATTER_FUNCTION(metal)
public:
    metal(const glm::vec3& a, float fuz) : albedo(a), fuzzy(fuz < 1 ? fuz : 1) {}
public:
    glm::vec3 albedo;
    float fuzzy;
};

class dielectric : public material
{
    DECALRE_SCATTER_FUNCTION(dielectric)
public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}
private:
    static float reflectance(float cosine, float ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
private:
    float ir;
};

