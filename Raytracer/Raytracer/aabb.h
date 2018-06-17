#pragma once
#ifndef AABBH
#define AABBH
#include "ray.h"
#include "hitable.h"
/* Bounding Box Class and methods  */
/* ffmin/max is better because it doesnt deal with NaNs and infinities (we dont need it)*/
inline float ffmin(float a, float b) {
	return a < b ? a : b;
}
inline float ffmax(float a, float b) {
	return a > b ? a : b;
}
class aabb {
public:
	vec3 _min;
	vec3 _max;
	bool b_empty_box;
	aabb() {}
	aabb(const vec3& a, const vec3& b, bool empty) {
		_min = a;
		_max = b;
		b_empty_box = empty;
	}
	vec3 min() const {
		return _min;
	}
	vec3 max() const {
		return _max;
	}
	bool hit(const ray& r, float tmin, float tmax) const {
		if(!b_empty_box){
			for (int dim = 0; dim < 3; dim++) {
				float invD = 1.0f / r.direction()[dim];
				float t0 = (min()[dim] - r.origin()[dim])*invD;
				float t1 = (max()[dim] - r.origin()[dim])*invD;
				if (invD < 0)
					std::swap(t0, t1);
				tmin = t0 > tmin ? t0 : tmin;
				tmax = t1 < tmax ? t1 : tmax;
				if (tmax <= tmin)
					return false;
			}
			return true;
		}
		return false;
	}

	static aabb surrounding_box(aabb box0, aabb box1) {
		if (box0.b_empty_box)
			return box1;
		else if (box1.b_empty_box)
			return box0;
		else {
			vec3 small(fmin(box0.min().x(), box1.min().x()),
				fmin(box0.min().y(), box1.min().y()),
				fmin(box0.min().z(), box1.min().z()));
			vec3 big(fmax(box0.max().x(), box1.max().x()),
				fmax(box0.max().y(), box1.max().y()),
				fmax(box0.max().z(), box1.max().z()));
			return aabb(small, big,false );
		}
	}
};

#endif