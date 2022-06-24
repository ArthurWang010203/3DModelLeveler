#include "Facet.h"

Facet::Facet(const struct Vertex& a_, const struct Vertex& b_, const struct Vertex& c_)
{
	std::memcpy(&a, &a_, sizeof(a_));
	std::memcpy(&b, &b_, sizeof(a_));
	std::memcpy(&c, &c_, sizeof(a_));
}

Facet::Facet(const struct Vertex& a_, const struct Vertex& b_, const struct Vertex& c_, const struct Vertex& normal_)
{
	std::memcpy(&a, &a_, sizeof(a_));
	std::memcpy(&b, &b_, sizeof(a_));
	std::memcpy(&c, &c_, sizeof(a_));
	std::memcpy(&normal, &normal_, sizeof(normal_));
}

auto Facet::Area() -> float
{
	float sideA = Distance(a, b);
	float sideB = Distance(b, c);
	float sideC = Distance(a, c);
	float semiPerim = (sideA + sideB + sideC) / 2;
	float rt = sqrt(std::fabs(semiPerim * (semiPerim - sideA) * (semiPerim - sideB) * (semiPerim - sideC) * 1.0));
	return rt;
}

auto Facet::RotateFacet(const Vertex& axis, float angle_rad) -> void
{
	// https://stackoverflow.com/questions/43623549/how-to-rotate-a-vector-around-a-particular-axis
	float cos_a = std::cos(angle_rad);
	float sin_a = std::sin(angle_rad);
	float comp_a = 1 - cos_a;
	float zero_zero = cos_a + axis.x * axis.x * comp_a;
	float zero_one = axis.x * axis.y * comp_a - axis.z * sin_a;
	float zero_two = axis.y * sin_a + axis.x * axis.z * comp_a;
	float one_zero = axis.z * sin_a + axis.x * axis.y * comp_a;
	float one_one = cos_a + axis.y * axis.y * comp_a;
	float one_two = -axis.x * sin_a + axis.y * axis.z * comp_a;
	float two_zero = -axis.y * sin_a + axis.x * axis.z * comp_a;
	float two_one = axis.x * sin_a + axis.y * axis.z * comp_a;
	float two_two = cos_a + axis.z * axis.z * comp_a;
	float a_0 = this->a.x * zero_zero + this->a.y * zero_one + this->a.z * zero_two;
	a_0 = compare_float(a_0, 0.0) ? 0.0 : a_0;
	float a_1 = this->a.x * one_zero + this->a.y * one_one + this->a.z * one_two;
	a_1 = compare_float(a_1, 0.0) ? 0.0 : a_1;
	float a_2 = this->a.x * two_zero + this->a.y * two_one + this->a.z * two_two;
	a_2 = compare_float(a_2, 0.0) ? 0.0 : a_2;
	Vertex a_(a_0, a_1, a_2);
	float b_0 = this->b.x * zero_zero + this->b.y * zero_one + this->b.z * zero_two;
	b_0 = compare_float(b_0, 0.0) ? 0.0 : b_0;
	float b_1 = this->b.x * one_zero + this->b.y * one_one + this->b.z * one_two;
	b_1 = compare_float(b_1, 0.0) ? 0.0 : b_1;
	float b_2 = this->b.x * two_zero + this->b.y * two_one + this->b.z * two_two;
	b_2 = compare_float(b_2, 0.0) ? 0.0 : b_2;
	Vertex b_(b_0, b_1, b_2);
	float c_0 = this->c.x * zero_zero + this->c.y * zero_one + this->c.z * zero_two;
	c_0 = compare_float(c_0, 0.0) ? 0.0 : c_0;
	float c_1 = this->c.x * one_zero + this->c.y * one_one + this->c.z * one_two;
	c_1 = compare_float(c_1, 0.0) ? 0.0 : c_1;
	float c_2 = this->c.x * two_zero + this->c.y * two_one + this->c.z * two_two;
	c_2 = compare_float(c_2, 0.0) ? 0.0 : c_2;
	Vertex c_(c_0, c_1, c_2);
	// get cross product
	Vertex vect_1 = a_ - b_;
	Vertex vect_2 = b_ - c_;
	Vertex& new_norm = cross_Product(vect_1, vect_2);
	new_norm.normalize();
	this->a = a_;
	this->b = b_;
	this->c = c_;
	this->normal = new_norm;
}