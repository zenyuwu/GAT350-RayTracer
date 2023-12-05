#pragma once

template <typename T>
inline T lerp(const T& a, const T& b, float t) {
	return (a * (1 - t)) + (t * b);
}

inline glm::vec3 cross(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::vec3{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};
}

inline float dot(const glm::vec3& v1, const glm::vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
	glm::vec3 r = v - (n * dot(n, v)) * glm::vec3{ 2,2,2 };
	return r;
}