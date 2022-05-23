#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

class material;

struct HitRecord {
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	inline void setFaceNormal(const ray& r, const vec3& outwardNormal) {
		front_face = dot(r.direction(), outwardNormal) < 0;
		normal = front_face ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif