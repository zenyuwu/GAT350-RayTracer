#include "Scene.h"
#include "Canvas.h"
#include "MathUtils.h"
#include "Random.h"
#include "Camera.h"
#include "Object.h"
#include <iostream>
#include <iomanip>

void Scene::Render(Canvas& canvas)
{
	//cast ray for each point (pixel) on the canvas
	for (int y = 0; y < canvas.GetSize().y; y++) {
		for (int x = 0; x < canvas.GetSize().x; x++) {
			//create vec2 pixel from canvas x,y
			glm::vec2 pixel = glm::vec2{ x, y };
			//get normalized (0 - 1) point coordinates from pixel
			glm::vec2 point = pixel / canvas.GetSize();
			//flip y
			point.y = 1.0f - point.y;

			//
			ray_t ray = m_camera->GetRay(point);

			raycastHit_t raycastHit;
			color3_t color = Trace(ray, 0, 100, raycastHit);
			canvas.DrawPoint(pixel, color4_t(color, 1));
		}
	}
}

void Scene::Render(Canvas& canvas, int numSamples, int depth)
{
	//cast ray for each point (pixel) on the canvas
	for (int y = 0; y < canvas.GetSize().y; y++) {
		for (int x = 0; x < canvas.GetSize().x; x++) {
			//create vec2 pixel from canvas x,y
			glm::vec2 pixel = glm::vec2{ x, y };

			//set initial color
			color3_t color{ 0 };
			//cast a ray for each sample, accumulate color value for each sample
			//each ray will have a random offset
			for (int sample = 0; sample < numSamples; sample++) {
				//get normalized (0-1) point coords from pixel
				//add random x and y offset
				glm::vec2 point = (pixel + glm::vec2{ random01(), random01() }) / canvas.GetSize();
				//flip y
				point.y = 1.0f - point.y;

				//create ray from camera
				ray_t ray = m_camera->GetRay(point);

				//cast ray into scene
				//ADD color value from trace
				raycastHit_t raycastHit;
				color += Trace(ray, 0, 100, raycastHit, depth);
			}
			
			//draw color to canvas point (pixel)
			//get average color (average = (color + color + color) / numSamples)
			color /= numSamples;

			canvas.DrawPoint(pixel, color4_t(color, 1));
			//std::cout << "uwu" << "%\n";
		}
		std::cout << std::setprecision(2) << std::setw(5) << "y" << ((y / canvas.GetSize().y) * 100) << "%\n";
	}
}
color3_t Scene::Trace(const ray_t& ray)
{
	glm::vec3 direction = glm::normalize(ray.direction);

	// set scene sky color
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);
	return color;
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit)
{
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : m_objects)
	{
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))
		{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// if ray hit object, scatter (bounce) ray and check for next hit
	if (rayHit)
	{
		ray_t scattered;
		color3_t color;

		if (raycastHit.material->Scatter(ray, raycastHit, color, scattered))
		{
			//return color3_t{ raycastHit.distance * 0.08f };
			//return raycastHit.normal;
			return color;
		}
		else
		{
			return raycastHit.material->GetEmissive();
		}
	}

	// if ray not hit, return scene sky color
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);

	return color;

	return color3_t();
}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit, int depth)
{
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : m_objects)
	{
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))
		{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// if ray hit object, scatter (bounce) ray and check for next hit
	if (rayHit)
	{
		ray_t scattered;
		color3_t color;

		if (depth > 0 && raycastHit.material->Scatter(ray, raycastHit, color, scattered)) {
			return color * Trace(scattered, minDistance, maxDistance, raycastHit, depth - 1);
		}
		else {
			return raycastHit.material->GetEmissive();
		}

		
	}

	// if ray not hit, return scene sky color
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);

	return color;

	return color3_t();
}


//if (raycastHit.material->Scatter(ray, raycastHit, color, scattered))
		//{
		//	//return color3_t{ raycastHit.distance * 0.08f };
		//	return raycastHit.normal;
		//	// return color;
		//}
		//else
		//{
		//	return color3_t{ 0, 0, 0 };
		//}
