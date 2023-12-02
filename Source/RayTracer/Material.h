#include "Ray.h"
#include "Color.h"
#include "Random.h"
class Material
{
public:
	virtual bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& color, ray_t& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : m_albedo{ albedo } {}
    bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& color, ray_t& scattered) const override {
        glm::vec3 target = raycastHit.point + raycastHit.normal + randomInUnitSphere();
        glm::vec3 direction = glm::normalize(target - raycastHit.point);
        scattered = { raycastHit.point, direction };
        color = m_albedo;
        return true;
    }

protected:
	color3_t m_albedo;
};