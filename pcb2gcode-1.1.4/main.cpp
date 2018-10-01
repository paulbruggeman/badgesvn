/*
 * This file is part of pcb2gcode.
 *
 * Copyright (C) 2009, 2010, 2011 Patrick Birnzain <pbirnzain@users.sourceforge.net> and others
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


#include <iostream>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::fstream;

#include <glibmm/ustring.h>
using Glib::ustring;

#include <glibmm/init.h>
#include <gdkmm/wrap_init.h>

#include "gerberimporter.hpp"
#include "surface.hpp"
#include "ngc_exporter.hpp"
#include "board.hpp"
#include "drill.hpp"
#include "options.hpp"
#include "svg_exporter.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include <fstream>
#include <sstream>


int main( int argc, char* argv[] )
{
	Glib::init();
	Gdk::wrap_init();

	options::parse( argc, argv );
	po::variables_map& vm = options::get_vm();

	if( vm.count("version") ) {
		cout << PACKAGE_STRING << endl;
		exit(0);
	}

	if( vm.count("help") ) {
		cout << options::help();
		exit(0);

		// cout << endl << "If you're new to pcb2gcode and CNC milling, please don't forget to read the attached documentation! "
		//      << "It contains lots of valuable hints on both using this program and milling circuit boards." << endl;
	}

	double unit=1;
	if( vm.count("metric") ) {
		unit=1./25.4;
	}
	options::check_parameters();


	// prepare environment
	shared_ptr<Isolator> isolator;
	if( vm.count("front") || vm.count("back") ) {
		isolator = shared_ptr<Isolator>( new Isolator() );
		isolator->tool_diameter = vm["offset"].as<double>() * 2*unit;
		isolator->zwork = vm["zwork"].as<double>()*unit;
		isolator->zsafe = vm["zsafe"].as<double>()*unit;
		isolator->feed = vm["mill-feed"].as<double>()*unit;
		isolator->speed = vm["mill-speed"].as<int>();
		isolator->zchange = vm["zchange"].as<double>()*unit;
		isolator->extra_passes = vm.count("extra-passes")?vm["extra-passes"].as<int>():0;
	}

	shared_ptr<Cutter> cutter;
	if( vm.count("outline") || (vm.count("drill") && vm.count("milldrill")) ) {
		cutter = shared_ptr<Cutter>( new Cutter() );
		cutter->tool_diameter = vm["cutter-diameter"].as<double>()*unit;
		cutter->zwork = vm["zcut"].as<double>()*unit;
		cutter->zsafe = vm["zsafe"].as<double>()*unit;
		cutter->feed = vm["cut-feed"].as<double>()*unit;
		cutter->speed = vm["cut-speed"].as<int>();
		cutter->zchange = vm["zchange"].as<double>()*unit;
		cutter->do_steps = true;
		cutter->stepsize = vm["cut-infeed"].as<double>()*unit;
	}

	shared_ptr<Driller> driller;
	if( vm.count("drill") ) {
		driller = shared_ptr<Driller>( new Driller() );
		driller->zwork = vm["zdrill"].as<double>()*unit;
		driller->zsafe = vm["zsafe"].as<double>()*unit;
		driller->feed = vm["drill-feed"].as<double>()*unit;
		driller->speed = vm["drill-speed"].as<int>();
		driller->zchange = vm["zchange"].as<double>()*unit;
	}

	// prepare custom preamble
	string preamble, postamble;
	if( vm.count("preamble") )
	{
		string name = vm["preamble"].as<string>();
		fstream in(name.c_str(),fstream::in);
		if(!in.good())
		{
			cerr << "Cannot read preamble file \"" << name << "\"" << endl;
			exit(1);
		}
		string tmp((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		preamble = tmp + "\n\n";
	}

	if( vm.count("postamble") )
	{
		string name = vm["postamble"].as<string>();
		fstream in(name.c_str(),fstream::in);
		if(!in.good())
		{
			cerr << "Cannot read preamble file \"" << name << "\"" << endl;
			exit(1);
		}
		string tmp((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		postamble = tmp + "\n\n";
	}

	shared_ptr<Board> board( new Board( vm["dpi"].as<int>(), vm.count("fill-outline"), vm.count("fill-outline") ? vm["outline-width"].as<double>() * unit : INFINITY ));

	// this is currently disabled, use --outline instead
	if( vm.count("margins") )
		board->set_margins( vm["margins"].as<double>() );

	// load files
	try
	{
		// import layer files, create surface
		cout << "Importing front side... ";
		try {
			string frontfile = vm["front"].as<string>();
			boost::shared_ptr<LayerImporter> importer( new GerberImporter(frontfile) );
			board->prepareLayer( "front", importer, isolator, false, vm.count("mirror-absolute") );
			cout << "done\n";
		} catch( import_exception& i ) {
			cout << "error\n";
		} catch( boost::exception& e ) {
			cout << "not specified\n";
		}

		cout << "Importing back side... ";
		try {
			string backfile = vm["back"].as<string>();
			boost::shared_ptr<LayerImporter> importer( new GerberImporter(backfile) );
			board->prepareLayer( "back", importer, isolator, true, vm.count("mirror-absolute") );
			cout << "done\n";
		} catch( import_exception& i ) {
			cout << "error\n";
		} catch( boost::exception& e ) {
			cout << "not specified\n";
		}

		cout << "Importing outline... ";
		try {
			string outline = vm["outline"].as<string>();
			boost::shared_ptr<LayerImporter> importer( new GerberImporter(outline) );
			board->prepareLayer( "outline", importer, cutter, !vm.count("front"), vm.count("mirror-absolute") );
			cout << "done\n";
		} catch( import_exception& i ) {
			cout << "error\n";
		} catch( boost::exception& e ) {
			cout << "not specified\n";
		}

	}
	catch(import_exception ie)
	{
		if( ustring const* mes = boost::get_error_info<errorstring>(ie) )
			std::cerr << "Import Error: " << *mes;
		else
			std::cerr << "Import Error: No reason given.";
	}

	
	//SVG EXPORTER
	shared_ptr<SVG_Exporter> svgexpo( new SVG_Exporter( board ) );
	
	try {
		board->createLayers();   // throws std::logic_error
		cout << "Calculated board dimensions: " << board->get_width() << "in x " << board->get_height() << "in" << endl;

		
		//SVG EXPORTER
		if( vm.count("svg") ) {
			cout << "Create SVG File ... " << vm["svg"].as<string>() << endl;
			svgexpo->create_svg( vm["svg"].as<string>() );
		}
		
		shared_ptr<NGC_Exporter> exporter( new NGC_Exporter( board ) );
		exporter->add_header( PACKAGE_STRING );
		if( vm.count("preamble") ) exporter->set_preamble(preamble);
		if( vm.count("postamble") ) exporter->set_postamble(postamble);
		
		//SVG EXPORTER
		if( vm.count("svg") ) exporter->set_svg_exporter( svgexpo );
		
		exporter->export_all(vm);
	} catch( std::logic_error& le ) {
		cout << "Internal Error: " << le.what() << endl;
	} catch( std::runtime_error& re ) {
	}

	if( vm.count("drill") ) {
		cout << "Converting " << vm["drill"].as<string>() << "... ";
		try {
			ExcellonProcessor ep( vm["drill"].as<string>(), board->get_min_x() + board->get_max_x() );
			ep.add_header( PACKAGE_STRING );
			if( vm.count("preamble") ) ep.set_preamble(preamble);
			if( vm.count("postamble") ) ep.set_postamble(postamble);

			//SVG EXPORTER
			if( vm.count("svg") ) ep.set_svg_exporter( svgexpo );
			
			
			if( vm.count("milldrill") )
				ep.export_ngc( vm["drill-output"].as<string>(), cutter, !vm.count("drill-front"), vm.count("mirror-absolute") );
			else
				ep.export_ngc( vm["drill-output"].as<string>(), driller, !vm.count("drill-front"), vm.count("mirror-absolute") );

			cout << "done.\n";
		} catch( drill_exception& e ) {
			cout << "ERROR.\n";
		}
	} else {
		cout << "No drill file specified.\n";
	}


}
