#pragma once

#include "Vertex.h"
#include <stdio.h>
#include <string.h>
#include <cstring>

struct Facet
{
	struct Vertex normal;
	struct Vertex a;
	struct Vertex b;
	struct Vertex c;
	Facet(const struct Vertex& a_, const struct Vertex& b_, const struct Vertex& c_);
	Facet(const struct Vertex& a_, const struct Vertex& b_, const struct Vertex& c_, const struct Vertex& normal_);
	auto Area() -> float;
	//bool operator==(const Facet& b);
	auto RotateFacet(const Vertex& axis, float angle_rad) -> void;
};