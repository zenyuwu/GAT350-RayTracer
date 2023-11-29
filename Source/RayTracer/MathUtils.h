#pragma once

template <typename T>
inline T lerp(const T& a, const T& b, float t) {
	return (a * (1 - t)) + (t * b);
}