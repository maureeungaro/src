// gstreamer
#include "gstreamerJLABSROFactory.h"

bool GstreamerJSROFactory::openConnection()
{
	ofile = new ofstream(gstreamer_definitions.name);
	
	// PRAGMA: need to check if file opened successfully
	return true;
}

bool GstreamerJSROFactory::closeConnection()
{
	ofile->close();
	delete ofile;
	
	// PRAGMA: need to check if file closed successfully
	return true;
}
