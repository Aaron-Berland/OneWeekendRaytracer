#pragma once
#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
class hitable_list : public hitable {
public:
	int list_size;
	hitable **list;
	hitable_list() {}
	hitable_list(hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
	hit_record temp_rec;
	bool hit_anything = false;
	float closest_sofar = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_sofar, temp_rec)) {
			hit_anything = true;
			closest_sofar = temp_rec.t;
			rec = temp_rec;
		}

	}
	return hit_anything;
}
#endif // !HITABLELISTH

