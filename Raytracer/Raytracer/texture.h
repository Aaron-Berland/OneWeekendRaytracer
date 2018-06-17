#pragma once
#ifndef TEXTUREH
#define TEXTUREH 
#include "vec3.h"
#include "perlin.h"
class texture {
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture {
public:
	vec3 color;
	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}

};

class checker_texture : public texture {
public:
	texture* odd;
	texture* even;
	checker_texture() {}
	checker_texture(texture* a,texture* b): even(a),odd(b){}
	virtual vec3 value(float u, float v, const vec3& p) const {
		float sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
		if (sines < 0)
			return even->value(u, v, p);
		else 
			return odd->value(u, v, p);
	}

};

class noise_texture : public texture {
public:
	noise_texture(){}
	noise_texture(float sc) : scale(sc){}
	virtual vec3 value(float u, float v, const vec3& p) const {
		return vec3(1, 1, 1)*0.5*(1 + sin(scale*p.x() + 5 * noise.turb(scale*p)));
		//return vec3(1, 1, 1)*0.5*(1 + sin(scale*p.z() + 5 * noise.turb(p)));
	}
	perlin noise;
	float scale;
};

class image_texture : public texture {
public:
	unsigned char* data;
	int nx;
	int ny;
	image_texture(){}
	image_texture(unsigned char* pixels, int A, int B) : data(pixels),nx(A), ny(B) {
	}
	virtual vec3 value(float u, float v, const vec3& p)  const {

		int i = u * nx;
		int j = (1 - v)*ny - 0.001;
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > nx - 1) i = nx - 1;
		if (j > ny - 1) j = ny - 1;
		//std::cout << "u coordinate : " << u << std::endl;
		//std::cout << "J v coordinate : " << v << std::endl;
		float r = int(data[3 * i + 3 * nx*j]) / 255.0;
		float g = int(data[3 * i + 3 * nx*j+1]) / 255.0;
		float b = int(data[3 * i + 3 * nx*j+2]) / 255.0;
		return vec3(r, g, b);
	}
};
#endif