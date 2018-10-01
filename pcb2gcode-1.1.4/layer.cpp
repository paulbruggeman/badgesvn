
#include "layer.hpp"

Layer::Layer( const string& name, shared_ptr<Surface> surface, shared_ptr<RoutingMill> manufacturer, bool backside, bool mirror_absolute )
{
	this->name = name;
	this->mirrored = backside;
	this->mirror_absolute = mirror_absolute;
	this->surface = surface;
	this->manufacturer = manufacturer;
}

#include <iostream>
using namespace std;

vector< shared_ptr<icoords> >
Layer::get_toolpaths()
{
	return surface->get_toolpath( manufacturer, mirrored, mirror_absolute );
}

shared_ptr<RoutingMill>
Layer::get_manufacturer()
{
	return manufacturer;
}

void
Layer::add_mask( shared_ptr<Layer> mask)
{
	surface->add_mask( mask->surface);
}
