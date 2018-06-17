#pragma once
#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"



class sphere :public hitable {
public:
	vec3 center;
	float radius;
	material *material_ptr;
	sphere() {}
	sphere(vec3 cen, float r, material* m_ptr) : center(cen), radius(r) {
		material_ptr = m_ptr;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const;

};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp>t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);			
			rec.normal = (rec.p - center) / radius;
			//TODO: Here one must changue from  view/world position to local position sistem )
			get_sphere_uv(rec.normal, rec.u, rec.v);
			rec.mat_ptr = material_ptr;
			return true;

		}
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			get_sphere_uv(rec.normal, rec.u, rec.v);
			rec.mat_ptr = material_ptr;
			return true;
		}

	}
	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb & outBox) const
{
	outBox = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius),false);
	return true;
}


#endif // !SPHEREH

