#pragma once

#include "GraphicCommonHeader.h"
#include "Vertex.h"
#include <algorithm>
#include <memory>
#include <vector>
GRAPHIC_NAMESPACE_BEGIN


#define TRIANGLE_COMPARE_FUNC(_DIR_)    \
    static bool cmp_##_DIR_(const Triangle& t1, const Triangle& t2) { return t1.center.##_DIR_ < t2.center.##_DIR_; }    \
    inline float min_##_DIR_() const { return std::min(p1.##_DIR_, std::min(p2.##_DIR_, p3.##_DIR_)); } \
    inline float max_##_DIR_() const { return std::max(p1.##_DIR_, std::max(p2.##_DIR_, p3.##_DIR_)); }

class Ray;
struct HitResult;
class Triangle
{
private:
    glm::vec3 p1,p2,p3;
    glm::vec3 center;
public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
        : p1(a), p2(b), p3(c) 
        { 
            center = (a + b + c)  / 3.0f;
        }
    TRIANGLE_COMPARE_FUNC(x)
    TRIANGLE_COMPARE_FUNC(y)
    TRIANGLE_COMPARE_FUNC(z)
    bool IsInnerPoint(const glm::vec3& p) const;
    float Hit(const Ray& r) const;
    static std::vector<Triangle> collections;
    static HitResult HitArray(const Ray& ray, const std::vector<Triangle>& triangles, int l, int r);

};

class BVHNode
{
private:
    std::shared_ptr<BVHNode> left = nullptr;
    std::shared_ptr<BVHNode> right = nullptr;
    int n = 0;
    int index = -1;
    glm::vec3 AA;
    glm::vec3 BB;
public:
    float HitAABB(const Ray& r);
    static std::shared_ptr<BVHNode> ROOT;
    static void UpdateRoot();
    static HitResult HitBVHTree(const Ray& ray, const std::vector<Triangle>& triangles = Triangle::collections, std::shared_ptr<BVHNode> node = ROOT);
    static std::shared_ptr<BVHNode> BuildBVHTree(std::vector<Triangle>& triangles, int l, int r, int n);
};

struct HitResult
{
    const Triangle* trianlge = nullptr;
    float distance = std::numeric_limits<float>::max();
};

class Ray
{
    friend class BVHNode;
private:
    glm::vec3 origin;
    glm::vec3 direction;
public:
    Ray(const glm::vec3& ori, const glm::vec3& dir)
        : origin(ori), direction( glm::normalize(dir))
    { }
    glm::vec3 at(float t) const {
        return origin + direction * t;
    }
    inline glm::vec3 pos()  const { return origin; }
    inline glm::vec3 dir() const { return direction; }
};

GRAPHIC_NAMESPACE_END