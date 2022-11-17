#include "RayTracing.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <random>
#include <thread>

#include "tools/ioUtil.h"
#include "graphic/Mesh.h"
#include "graphic/Texture.h"

#include "system/RenderSystem.h"
#include "system/ResourceSystem.h"
#include "engine/Engine.h"

namespace RayTracingInOneWeekend
{
void OutPutAnImage();
glm::vec3 RayColor(Ray& ray, const Hittable& world, int depth);
void WriteColor(std::stringstream& stream, const glm::vec3& color, int sample_per_pixel);
void OutPutAnImageWithRay();
void ShowImageInRenderWindow();
float RandomFloat();
float RandomFloat(float min, float max) { return min + (max - min)*RandomFloat(); }
glm::vec3 RandomVec3() { return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat()); }
glm::vec3 RandomVec3(float min, float max) { return glm::vec3(RandomFloat(min,max), RandomFloat(min,max), RandomFloat(min,max)); }
glm::vec3 RandomVec3InUnitSphere() 
{ 
    while(true)
    {
        glm::vec3 p = RandomVec3(-1,1);
        if (glm::length(p) >= 1) continue;
        return p;
    }
}
glm::vec3 RandomVec3InNormalDir(const glm::vec3& normal)
{
    glm::vec3 dir = RandomVec3InUnitSphere();
    if (glm::dot(dir, normal) > 0.0f)
    {
        return dir;
    }
    return -dir;
}
bool nearZero(const glm::vec3& v)
{
    static const float s = 1e-8;
    return std::abs(v.x) <= s && std::abs(v.y) <= s && std::abs(v.z) <= s;
}
class camera {
    public:
        camera(
            const glm::vec3& lookfrom,
            const glm::vec3& lookat,
            const glm::vec3&   vup,
            float vfov, // vertical field-of-view in degrees
            float aspect_ratio,
            float aperture,
            float focus_dist
        ) {
            auto theta = glm::radians(vfov);
            auto h = tan(theta/2);
            float viewport_height = 2.0 * h;
            float viewport_width = aspect_ratio * viewport_height;
            
            w = glm::normalize(lookfrom - lookat);
            u = glm::normalize(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal*0.5f - vertical*0.5f - focus_dist*w;

            lens_radius = aperture / 2;
        }

        Ray get_ray(float s, float t) const {
            return Ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal;
        glm::vec3 vertical;
        glm::vec3 u, v, w;
        float lens_radius;
};

};






void RayTracing::Update()
{
    //RayTracingInOneWeekend::OutPutAnImageWithRay();
    RayTracingInOneWeekend::ShowImageInRenderWindow();
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const
{
    glm::vec3 oc = r.origin() - center;
    auto a = glm::dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = glm::dot(oc,oc) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.mat = mat;
    glm::vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    
    return true;
}

bool Hittable_List::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const
{
    Hit_Record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool lambertian::scatter(const Ray& r_in, const Hit_Record& rec, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 scatter_direction = rec.normal + RayTracingInOneWeekend::RandomVec3InUnitSphere();

    // Catch degenerate scatter direction
    if (RayTracingInOneWeekend::nearZero(scatter_direction))
        scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}

bool metal::scatter(const Ray& r_in, const Hit_Record& rec, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 reflected = glm::reflect(r_in.direction(), rec.normal);
    scattered = Ray(rec.p, reflected + fuzzy * RayTracingInOneWeekend::RandomVec3InNormalDir(rec.normal));
    attenuation = albedo;
    return (glm::dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(const Ray& r_in, const Hit_Record& rec, glm::vec3& attenuation, Ray& scattered) const
{
    attenuation = glm::vec3(1.0, 1.0, 1.0);
    float refraction_in = 1.0f;

    float refraction_ratio = rec.front_face ? (refraction_in/ir) : (ir/refraction_in);

    glm::vec3 unit_direction = glm::normalize(r_in.direction());
    float cos_theta = fmin(glm::dot(-unit_direction, rec.normal), 1.0);
    float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    glm::vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > RayTracingInOneWeekend::RandomFloat())
        direction = glm::reflect(unit_direction, rec.normal);
    else
    {
        direction = glm::refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = Ray(rec.p, direction);
    return true;
}


namespace RayTracingInOneWeekend
{

void OutPutAnImage()
{
    static int num = 0;
    if (num > 0)
        return;
    
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);


    std::stringstream imageRawData;

    // Render
    imageRawData << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            imageRawData << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    Util::writeFile_Native((std::string(_RESOURCE_PATH_) + "/output/img.ppm").c_str(), imageRawData.str().c_str(), imageRawData.str().size());
}

glm::vec3 RayColor(Ray& ray, const Hittable& world, int depth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return glm::vec3(0,0,0);
    Hit_Record rec;
    if (world.hit(ray, 0.001f, std::numeric_limits<float>::infinity(), rec)) {
        Ray scattered;
        glm::vec3 attenuation;
        if (rec.mat && rec.mat->scatter(ray, rec, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, world, depth - 1);
        }
        return glm::vec3(0,0,0);
    }

    float t = 0.5*(ray.direction().y + 1.0);
    return (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
}

void WriteColor(std::stringstream& stream, const glm::vec3& color, int sample_per_pixel)
{
    float r = color.x;
    float g = color.y;
    float b = color.z;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / sample_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    stream << static_cast<int>(256 * std::clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0f, 0.999f)) << '\n';
}

void WriteColorBuffer(unsigned char* buffer, int x, int y, int width, int height, const glm::vec3& color, int sample_per_pixel)
{
    float r = color.x;
    float g = color.y;
    float b = color.z;
    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / sample_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    unsigned char col[4] = 
    {
        static_cast<unsigned char>(static_cast<int>(255 * std::clamp(r, 0.0f, 0.999f))), 
        static_cast<unsigned char>(static_cast<int>(255 * std::clamp(g, 0.0f, 0.999f))), 
        static_cast<unsigned char>(static_cast<int>(255 * std::clamp(b, 0.0f, 0.999f))), 
        255
    };
    memcpy(buffer + y * width * 4 + x * 4, col, 4);
}

float RandomFloat()
{
    static std::uniform_real_distribution<float> distribution(0.0, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

Hittable_List RandomScene() {
    Hittable_List world;

    auto ground_material = new lambertian(glm::vec3(0.5, 0.5, 0.5));
    world.add(new Sphere(glm::vec3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomFloat();
            glm::vec3 center(a + 0.9*RandomFloat(), 0.2, b + 0.9*RandomFloat());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                material* sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = RandomVec3InUnitSphere() * RandomVec3InUnitSphere();
                    sphere_material = new lambertian(albedo);
                    world.add(new Sphere(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = RandomVec3(0.5,1);
                    auto fuzz = RandomFloat(0, 0.5);
                    sphere_material = new metal(albedo, fuzz);
                    world.add(new Sphere(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = new dielectric(1.5);
                    world.add(new Sphere(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = new dielectric(1.5);
    world.add(new Sphere(glm::vec3(0, 1, 0), 1.0, material1));

    auto material2 = new lambertian(glm::vec3(0.4, 0.2, 0.1));
    world.add(new Sphere(glm::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = new metal(glm::vec3(0.7, 0.6, 0.5), 0.0);
    world.add(new Sphere(glm::vec3(4, 1, 0), 1.0, material3));

    return world;
}



void ShowImageInRenderWindow()
{
    // Image
    static auto aspect_ratio = 16.0 / 9.0;
    static int image_width = Engine::GetWindowWidth();
    static int image_height = static_cast<int>(image_width / aspect_ratio);
    static int samples_per_pixel = 8;
    static const int max_depth = 10;
    static unsigned char* image_data = new unsigned char[image_width * image_height * 4];

    static int num = 0;
    static Graphic::Texture texture(image_data, image_width, image_height);
    static Graphic::Mesh mesh(Graphic::VertexStream::quad, "2D", std::vector<Graphic::Texture*>{&texture});
    if (num > 0)
    {
        texture.UpdateBuffer(image_data);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        mesh.draw();
        return;
    }

    mesh.SetShaderParam("model", glm::mat4(1.0f));
    num++;
    memset(image_data, 0, image_width*image_height*4);

        // World
        Hittable_List world = RandomScene();;
        //auto material_ground = lambertian(glm::vec3(0.8, 0.8, 0.0));
        //auto material_center = metal(glm::vec3(0.2, 0.7, 0.2), 0.0f);
        //auto material_left   = dielectric(1.3f);
        //auto material_right  = metal(glm::vec3(0.8, 0.6, 0.2),0.0f);
//
        //Sphere s1(glm::vec3( 0.0, -100.5, -1.0), 100.0, &material_ground);
        //Sphere s2(glm::vec3( 0.0,    0.0, -1.0),   0.5, &material_center);
        //Sphere s3(glm::vec3(-1.0,    0.0, -1.0),   0.5, &material_left);
        //Sphere s4(glm::vec3( 1.0,    0.0, -1.0),   0.5, &material_right);
//
        //world.add(&s1);
        //world.add(&s2);
        //world.add(&s3);
        //world.add(&s4);



        // Camera
        glm::vec3 lookfrom(13,2,3);
        glm::vec3 lookat(0,0,0);
        glm::vec3 vup(0,1,0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    auto func = [=](int width_from, int width_to)
    {
        // Render
        for (int j = image_height - 1; j >= 0; --j) {
            //std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = width_from; i <= width_to; ++i) {
                glm::vec3 pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    float u = (i + RandomFloat()) / (image_width-1);
                    float v = (j + RandomFloat()) / (image_height-1);
                    Ray r = cam.get_ray(u, v);
                    pixel_color += RayColor(r, world, max_depth);
                }
                WriteColorBuffer(image_data, i, j, image_width, image_height, pixel_color, samples_per_pixel);
            }
        }
        //Util::writeFile_Native((std::string(_RESOURCE_PATH_) + "/output/img_withRay.ppm").c_str(), imageRawData.str().c_str(), imageRawData.str().size());
    };
    
    int threadNum = 24;
    int each_width = image_width / (threadNum + 1);
    int start = 0;
    int end = each_width;
    while(end < image_width - 1)
    {
        std::cout << start << " " << end << std::endl;
        std::thread p(func, start, end);
        p.detach();
        start = end + 1;
        end = end + each_width;
    }

    if (start <= image_width - 1)
    {
        std::cout << start << " " << image_width - 1 << std::endl;
        std::thread p(func, start, image_width - 1);
        p.detach();
    }
}

void OutPutAnImageWithRay()
{
    static int num = 0;
    if (num > 0)
        return;
    num++;


    // Image
    const int image_width = Engine::GetWindowWidth();
    const int image_height = Engine::GetWindowHeight();
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    
    // World
    Hittable_List world;
    Sphere s1(glm::vec3(0,0,-1), 0.5, nullptr);
    Sphere s2(glm::vec3(0,-100.5,-1), 100, nullptr);
    Sphere s3(glm::vec3(0,0,-3),1,nullptr);
    world.add(&s1);
    world.add(&s2);
    world.add(&s3);

    // Camera
    //camera cam(glm::vec3(-2,2,1), glm::vec3(0,0,-1), glm::vec3(0,1,0), 20, 16.0f/9.0f);


    // Render
    std::stringstream imageRawData;
    imageRawData << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        //std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            glm::vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (i + RandomFloat()) / (image_width-1);
                float v = (j + RandomFloat()) / (image_height-1);
                //Ray r = cam.get_ray(u, v);
                //pixel_color += RayColor(r, world, max_depth);
            }

            WriteColor(imageRawData, pixel_color, samples_per_pixel);
        }
    }
    Util::writeFile_Native((std::string(_RESOURCE_PATH_) + "/output/img_withRay.ppm").c_str(), imageRawData.str().c_str(), imageRawData.str().size());
}


}