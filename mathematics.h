#pragma once

#include "i_interfaces.h"
#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

namespace math {
	void normalize_angles(Vector& angles);
	float angle_distance(float firstangle, float secondangle);
	float lowerbody_rotate(float lby, float yaw);
	void normalize_angle(QAngle& angles);
	void normalize_vector(Vector& vector);
	float normalize_yaw(float f);
	float normalize_pitch(float pitch);
	float vector_normalize(Vector& v);
	void clamp_angles(Vector& angles);
	void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	Vector calculate_angle(Vector src, Vector dst);
	float get_fov(Vector viewangles, Vector aim_angle);
	void angle_vectors(const Vector& angles, Vector& forward);
	Vector get_transformation(const Vector& in, const matrix3x4_t& matrix);
	Vector i_angle_t_vector_i(const Vector& angles);
	void angle_vectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);
	void angle_vectors(const QAngle& angles, Vector& forward);
	Vector angle_vectors(Vector& angle);
	void vector_angles(const Vector& forward, Vector& angles);
	void vector_angles(const Vector& forward, QAngle& angles);
	void vector_angles(const Vector& forward, Vector& up, Vector& angles);
	Vector cross_product(const Vector& a, const Vector& b);
	float distance_to_ray(const Vector& pos, const Vector& ray_start, const Vector& ray_end, float* along = 0, Vector* point_on_ray = 0);
	float dot_product(const Vector& a, const Vector& b);
	float approach_angle(float fl_target, float fl_value, float fl_speed);
	void rotate_movement(float yaw);
	Vector vector_angles(const Vector& forward);

	float random_float(float min, float max);
	void random_seed(int seed);

	template<class T, class U>
	static T clamp(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	inline float clamp_yaw(float yaw)
	{
		while (yaw > 180.f)
			yaw -= 360.f;
		while (yaw < -180.f)
			yaw += 360.f;
		return yaw;
	}
}