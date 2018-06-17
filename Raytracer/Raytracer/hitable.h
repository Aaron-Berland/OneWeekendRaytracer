#pragma once
#ifndef HITABLEH
#define HITABLEH
#include "ray.h"
#include "aabb.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

class material;
void get_sphere_uv(const vec3& p, float& u, float& v) {
	float phi = atan2(p.y(), p.x());
	float thetha = asin(p.z());
	u = (phi + M_PI) / (2 *M_PI);
	v = (thetha + M_PI / 2) / M_PI;
}
struct hit_record {
	float t;
	vec3 p;
	float u;
	float v;
	vec3 normal;
	material *mat_ptr;
};
class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const = 0;
};

class flip_normals : public hitable {
public:
	hitable* ptr;
	flip_normals(hitable* ahitable) : ptr(ahitable) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const {
		return ptr->bounding_box(t0, t1, outBox);
	}
};

class translate : public hitable {
public:
	vec3 translation;
	hitable* translated_hitable;
	translate(hitable* p, const vec3& translation_vector) : translation(translation_vector), translated_hitable(p){}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const;
};
bool translate::bounding_box(float t0, float t1, aabb& outBox) const {
	if (translated_hitable->bounding_box(t0, t1,outBox) ){
		outBox = aabb(outBox.min() + translation, outBox.max() + translation, false);
		return true;
	}
	else
		return false;


}
bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	ray moved_r(r.origin() - translation, r.direction(), r.time());
	if (translated_hitable->hit(moved_r, t_min, t_max, rec)) {
		rec.p += translation;
		return true;
	}
	else
		return false;

}

class rotate_y : public hitable {
public:
	rotate_y(hitable *p, float angle);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = bbox; return hasbox;
	}
	hitable *ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	aabb bbox;
};

rotate_y::rotate_y(hitable *p, float angle) : ptr(p) {
	float radians = (M_PI / 180.) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = i*bbox.max().x() + (1 - i)*bbox.min().x();
				float y = j*bbox.max().y() + (1 - j)*bbox.min().y();
				float z = k*bbox.max().z() + (1 - k)*bbox.min().z();
				float newx = cos_theta*x + sin_theta*z;
				float newz = -sin_theta*x + cos_theta*z;
				vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = aabb(min, max, bbox.b_empty_box);
}

bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
	origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];
	direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
	direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];
	ray rotated_r(origin, direction, r.time());
	if (ptr->hit(rotated_r, t_min, t_max, rec)) {
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
		p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];
		normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
		normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
		return false;
}
#endif