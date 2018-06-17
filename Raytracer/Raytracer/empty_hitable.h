#pragma once
#ifndef EMPTYHITABLEH
#define EMPTYHITABLEH

#include "hitable.h"

class empty_hitable : public hitable {
public:
	empty_hitable() {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const;
	
};
bool empty_hitable::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	return false;
}
bool empty_hitable::bounding_box(float t0, float t1, aabb& outBox)const {
	outBox = aabb(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), true);
	return true;
}

#endif