#pragma once
#ifndef MOVINGSPHEREH
#define MOVINGSPHEREH

#include "hitable.h"

class moving_sphere :public hitable {
public:
	vec3 center1,center0;
	float radius;
	material *material_ptr;
	float time1, time0;
	moving_sphere() {}
	vec3 center(float time) const;
	moving_sphere(vec3 cen0,vec3 cen1, float t0,float t1, float r, material* m_ptr) : center0(cen0),center1(cen1),time0(t0),time1(t1), radius(r) {
		material_ptr = m_ptr;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const;

};

vec3 moving_sphere::center(float time) const{
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}
bool moving_sphere::bounding_box(float t0, float t1, aabb& outBox) const {
	
	aabb box1 = aabb(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius),false);
	aabb box2 = aabb(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius),false);
	outBox = aabb::surrounding_box(box1, box2);
	return true;
}
bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp>t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = material_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;

		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = material_ptr;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			return true;
		}

	}
	return false;
}
#endif