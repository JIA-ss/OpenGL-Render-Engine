#include "Triangle.h"

using namespace Graphic;
std::vector<Triangle> Triangle::collections = std::vector<Triangle>();

bool Triangle::IsInnerPoint(const glm::vec3& p) const
{
    glm::vec3 v0 = p2-p1;
    glm::vec3 v1 = p3-p1;
    glm::vec3 v2 = p -p1;
    /*
        u = ((v1•v1)(v2•v0)-(v1•v0)(v2•v1)) / ((v0•v0)(v1•v1) - (v0•v1)(v1•v0))
        v = ((v0•v0)(v2•v1)-(v0•v1)(v2•v0)) / ((v0•v0)(v1•v1) - (v0•v1)(v1•v0))
    */
    float dot00 = glm::dot(v0,v0);
    float dot01 = glm::dot(v0,v1);
    float dot02 = glm::dot(v0,v2);
    float dot11 = glm::dot(v1,v1);
    float dot12 = glm::dot(v1,v2);
    
    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
    if (u < 0 || u > 1) // if u out of range, return directly
        return false ;
    

    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
    if (v < 0 || v > 1) // if v out of range, return directly
        return false ;

    return u + v <= 1 ;
}

float Triangle::Hit(const Ray& r) const
{
    glm::vec3 N = glm::cross(p2-p1,p3-p1);
    N = glm::normalize(N);

    float d_dot_n = glm::dot(r.dir(),N);
    if (std::abs(d_dot_n) <= 0.00001f)
        return -1.0f;
    N = d_dot_n < 0.0f ? -N : N;

    // (r.pos() + t*r.dir())-p1)*N = 0;
    float t = (glm::dot(p1, N) - glm::dot(r.pos(), N)) / glm::dot(r.dir(), N);
    if (t < 0.0005f)
        return -1.0f;
    
    if (IsInnerPoint(r.pos() + t * r.dir()))
        return t;
    
    return -1.0f;
}

HitResult Triangle::HitArray(const Ray& ray,const std::vector<Triangle>& triangles, int l, int r)
{
    HitResult res;
    for (int i = l; i <= r; i++)
    {
        float d = triangles[i].Hit(ray);
        if (d > 0.0f && d < res.distance)
        {
            res.distance = d;
            res.trianlge = &triangles[i];
        }
    }
    return res;
}

std::shared_ptr<BVHNode> BVHNode::ROOT = std::make_shared<BVHNode>();

float BVHNode::HitAABB(const Ray& r)
{
    // float tin_x = (AA.x - r.pos().x)/r.dir().x;
    // tin_y ... 
    glm::vec3 invdir = glm::vec3(1.0f/r.dir().x, 1.0f/r.dir().y, 1.0f/r.dir().z);
    glm::vec3 tin = (AA - r.pos()) * invdir;
    glm::vec3 tout = (BB - r.pos()) * invdir;
    
    glm::vec3 tmin = glm::vec3(std::min(tin.x, tout.x), std::min(tin.y, tout.y), std::min(tin.z, tout.z));
    glm::vec3 tmax = glm::vec3(std::max(tin.x, tout.x), std::max(tin.y, tout.y), std::max(tin.z, tout.z));

    float t1 = std::min(tmax.x, std::min(tmax.y, tmax.z));
    float t0 = std::max(tmin.x, std::max(tmin.y, tmin.z));

    return t1 >= t0 ? (t0 > 0 ? t0 : t1) : -1;
}

void BVHNode::UpdateRoot()
{
    ROOT = BuildBVHTree(Triangle::collections, 0, Triangle::collections.size() - 1, 5);
}

std::shared_ptr<BVHNode> BVHNode::BuildBVHTree(std::vector<Triangle>& triangles, int l, int r, int n)
{
    if (l > r || triangles.empty())
        return nullptr;
    
    std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
    node->AA = glm::vec3(std::numeric_limits<float>::max());
    node->BB = glm::vec3(std::numeric_limits<float>::min());
    for (int i = l; i <= r; i++)
    {
        node->AA.x = std::min(node->AA.x, triangles[i].min_x());
        node->AA.y = std::min(node->AA.y, triangles[i].min_y());
        node->AA.z = std::min(node->AA.z, triangles[i].min_z());

        node->BB.x = std::max(node->BB.x, triangles[i].max_x());
        node->BB.y = std::max(node->BB.y, triangles[i].max_y());
        node->BB.z = std::max(node->BB.z, triangles[i].max_z());
    }

    if (r-l+1 <= n)
    {
        node->n = r-l+1;
        node->index = l;
        return node;
    }

    int lenx = node->BB.x - node->AA.x;
    int leny = node->BB.y - node->AA.y;
    int lenz = node->BB.z - node->AA.z;

    if (lenx >= leny && lenx >= lenz)
    {
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, Triangle::cmp_x);
    }
    else if (leny >= lenx && leny >= lenz)
    {
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, Triangle::cmp_y);
    }
    else if (lenz >= lenx && lenz >= leny)
    {
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, Triangle::cmp_z);
    }

    int mid = (l + r / 2);
    node->left = BuildBVHTree(triangles, l, mid, n);
    node->right = BuildBVHTree(triangles, mid + 1, r, n);
    return node;
}



HitResult BVHNode::HitBVHTree(const Ray& ray, const std::vector<Triangle>& triangles, std::shared_ptr<BVHNode> node)
{
    if (node == nullptr)
        return {};
    
    if (node->n > 0)
    {
        return Triangle::HitArray(ray, triangles, node->index, node->index + node->n - 1);
    }

    float d1 = std::numeric_limits<float>::max();
    float d2 = d1;

    if (node->left)
        d1 = node->left->HitAABB(ray);
    if (node->right)
        d2 = node->right->HitAABB(ray);

    HitResult res1, res2;
    if (d1 > 0)
    {
        res1 = HitBVHTree(ray, triangles, node->left);
    }

    if (d2 > 0)
    {
        res2 = HitBVHTree(ray, triangles, node->right);
    }

    return res1.distance < res2.distance ? res1 : res2;

}