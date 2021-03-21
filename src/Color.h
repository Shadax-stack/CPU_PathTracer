#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <glm/glm.hpp>

template<typename T>
struct ColorRGB {
public:
	T R;
	T G;
	T B;

	ColorRGB(void) = default;

	template<typename T>
	ColorRGB(glm::tvec3<T> Value) : R(Value.r), G(Value.g), B(Value.b) {}

	ColorRGB(T IR, T IG, T IB) : R(IR), G(IG), B(IB) {}
};

template<typename T>
struct ColorRGBA {
public:
	T R;
	T G;
	T B;
	T A;
};;

using ColorRGB_8UI = ColorRGB<uint8_t>;
using ColorRGB_32F = ColorRGB<float>;

#endif