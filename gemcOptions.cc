// goptions
#include "goptions.h"

// options definitions
#include "gemcOptions.h"

// other modules goptions
#include "gsystemOptions.h"
#include "g4systemOptions.h"
#include "gstreamerOptions.h"
#include "g4displayOptions.h"
#include "eventDispenserOptions.h"


// c++
#include <iostream>
using namespace std;


namespace gemc {

	// returns array of options definitions
	vector<GOption> defineOptions()
	{
		vector<GOption> goptions;

		// add a "gui" switch
		goptions.push_back(GOption("gui", "use Graphical User Interface"));

		// number of threads. Default = 1
		json jsonNThreadOption = {
			{GNAME, "nthreads"},
			{GDESC, "set number of threads"},
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonNThreadOption));

		// general verbosity
		json jsonMessageVerbosity = {
			{GNAME, "verbosity"},
			{GDESC, GVERBOSITY_DESCRIPTION},
			{GDFLT, 1}
		};
		goptions.push_back(GOption(jsonMessageVerbosity));

		// run verbosity
		json jsonRunVerbosity = {
			{GNAME, "grunv"},
			{GDESC, GVERBOSITY_DESCRIPTION},
			{GDFLT, 1}
		};
		goptions.push_back(GOption(jsonRunVerbosity));

		// event log every N events
		json jsonELOGOption = {
			{GNAME, "elog"},
			{GDESC, "Log every N events"},
			{GDFLT, 1000}
		};
		goptions.push_back(GOption(jsonELOGOption));

		// event log every N events
		json jsonTLOGOption = {
			{GNAME, "tlog"},
			{GDESC, "Log only thread # given. 0 (default) means log all"},
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonTLOGOption));

		// other modules goptions
		goptions += gsystem::defineOptions();
		goptions += g4system::defineOptions();
		goptions += gstreamer::defineOptions();
		goptions += g4display::defineOptions();
		goptions += eventDispenser::defineOptions();

		return goptions;
	}


}


