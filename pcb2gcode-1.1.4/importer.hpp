
/*
 * This file is part of pcb2gcode.
 * 
 * Copyright (C) 2009, 2010 Patrick Birnzain <pbirnzain@users.sourceforge.net>
 * 
 * pcb2gcode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pcb2gcode is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with pcb2gcode.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMPORTER_H
#define IMPORTER_H

#include <glibmm/ustring.h>
using Glib::ustring;

#include <glibmm/refptr.h>
#include <cairomm/cairomm.h>
#include <gdk/gdkcairo.h>

#include <boost/exception/all.hpp>
struct import_exception : virtual std::exception, virtual boost::exception {};
typedef boost::error_info<struct tag_my_info, ustring> errorstring;

//! pure virtual base class for importers.
class LayerImporter
{
public:
	virtual gdouble get_width() = 0;
	virtual gdouble get_height() = 0;
	virtual gdouble get_min_x() = 0;
	virtual gdouble get_max_x() = 0;
	virtual gdouble get_min_y() = 0;
	virtual gdouble get_max_y() = 0;

	virtual void render(Cairo::RefPtr<Cairo::ImageSurface> surface,
			    const guint dpi, const double xoff, const double yoff)
		throw (import_exception) = 0;
	
};

#endif // IMPORTER_H
