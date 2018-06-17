#pragma once
#ifndef BVHH
#define BVHH
#include "hitable.h"
#include "hitable_list.h"
#include "empty_hitable.h"
#include "global_random.h"
int box_x_compare(const void * a, const void * b) {
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().x() - box_right.min().x() < 0.0)
		return -1;
	else
		return 1;
}

int box_y_compare(const void * a, const void * b)
{
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().y() - box_right.min().y() < 0.0)
		return -1;
	else
		return 1;
}
int box_z_compare(const void * a, const void * b)
{
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().z() - box_right.min().z() < 0.0)
		return -1;
	else
		return 1;
}
class bvh_node : public hitable {
public:
	hitable* right;
	hitable* left;
	aabb box;
	bvh_node(){}
	bvh_node(hitable **l, int n, float time0, float time1,int depth, int maxDepth);
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& outBox) const;

};

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	if (box.hit(r, t_min, t_max)) {
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right) {
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_right) {
			rec = right_rec;
			return true;
		}
		else if (hit_left) {
			rec = left_rec;
			return true;
		}
		else
			return false;
	}
	return false;
}
bool bvh_node::bounding_box(float t0, float t1, aabb& outBox) const {
	outBox = box;
	return true;
}
bvh_node::bvh_node(hitable **l, int n, float time0, float time1,int depth, int maxDepth) {
	int axis = int(3 * get_random());
	if (axis == 0)
		qsort(l, n, sizeof(hitable *), box_x_compare);
	else if (axis == 1)
		qsort(l, n, sizeof(hitable *), box_y_compare);
	else
		qsort(l, n, sizeof(hitable *), box_z_compare);
	if (n == 1) {
		left = right = l[0];
	}
	else if (n == 2) {
		left = l[0];
		right = l[1];
	}
	else if (depth >= maxDepth) {
		left = new hitable_list(l, n);
		right = new empty_hitable();
	}
	else {
		left = new bvh_node(l, n / 2, time0, time1,depth+1,maxDepth);
		right = new bvh_node(l + n / 2, n - n / 2, time0, time1, depth + 1, maxDepth);
	}
	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	box = aabb::surrounding_box(box_left, box_right);
}


#endif // !BVHH
