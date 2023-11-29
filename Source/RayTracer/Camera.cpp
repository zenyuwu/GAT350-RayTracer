#include "Camera.h"

Camera::Camera(const glm::vec3& eye, const glm::vec3& up, float fov, float aspectRatio)
{

}

void Camera::LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{

}

ray_t Camera::GetRay(const glm::vec2& point) const
{
	return ray_t();
}

void Camera::CalculateViewPlane()
{

}
