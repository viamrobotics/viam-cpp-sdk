#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <common/v1/common.pb.h>
#include <spatialmath/orientation.h>

#include <string>
#include <tuple>

struct GeometryConfig {
	std::string type;
	double x;
	double y;
	double z;
	double r;
	std::tuple<double, double, double> translation_offset;
	OrientationConfig orientation_config;
	std::string label;
	viam::common::v1::Geometry to_proto();
};

#endif
