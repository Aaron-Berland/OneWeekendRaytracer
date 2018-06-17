#pragma once
#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES
#include <math.h>
#include "ray.h"
#include "global_random.h"
class camera {
public:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	float lens_radius;
	vec3 u, v, w;
	float time0, time1;

	camera(){}

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,float aperture,float focal_dist, float t0,float t1) {
		time0 = t0;
		time1 = t1;
		lens_radius = aperture / 2.0;
		float half_angle = (vfov / 360.0)*M_PI;
		float half_height = focal_dist*tan(half_angle);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_height*v -half_width*u - focal_dist*w;
		horizontal = 2*half_width*u;
		vertical = 2*half_height*v;
		
	}
	ray get_ray(float t,float s) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = rd.x()*u + rd.y()*v;
		float time = time0 + (time1 - time0)*get_random();	
		return ray(origin +offset , lower_left_corner + t*horizontal + s*vertical - origin-offset,time);
	}

};
#endif // !CAMERAH

