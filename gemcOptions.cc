// goptions
#include "gutsConventions.h"
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

		goptions.push_back(GOption("stream", "Activate Streaming ReadOut"));

//		// default material to use when a material is not found
//		json jsonFrameTimeLength = {
//			{GNAME, "frameTimeLength"},
//			{GDESC, "Duration of a SRO frame"},
//			{GDFLT, UNINITIALIZEDNUMBERQUANTITY}
//		};
//
//		json jsonSRO = {
//			jsonFrameTimeLength
//		};
//
//		vector<string> help;
//		help.push_back("Define the GEMC streaming readout");
//		help.push_back("");
//		help.push_back("Example: -sro={frameTimeLength: 64*us; name: sro.txt; }");
//		help.push_back("");
//		help.push_back("Current available formats:");
//		help.push_back("");
//		help.push_back(" - TEXT");
//
//		// the last argument refers to "cumulative"
//		goptions.push_back(GOption("sro", "Streaming Readout Definitions", jsonSRO, help, false));


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
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonMessageVerbosity));

		// run verbosity
		json jsonRunVerbosity = {
			{GNAME, "grunv"},
			{GDESC, GVERBOSITY_DESCRIPTION},
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonRunVerbosity));

		// sensitivity verbosity
		json jsonSensitivityVerbosity = {
			{GNAME, "gsensitivityv"},
			{GDESC, GVERBOSITY_DESCRIPTION},
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonSensitivityVerbosity));

		// event log every N events
		json jsonELOGOption = {
			{GNAME, "elog"},
			{GDESC, "Log every N events"},
			{GDFLT, 1000}
		};
		goptions.push_back(GOption(jsonELOGOption));

		// location of plugins
		json jsonPluginPathOption = {
			{GNAME, "gpluginsPath"},
			{GDESC, "Directory containing the plugins"},
			{GDFLT, UNINITIALIZEDSTRINGQUANTITY}
		};
		goptions.push_back(GOption(jsonPluginPathOption));

		// event log every N events
		json jsonTLOGOption = {
			{GNAME, "tlog"},
			{GDESC, "Log only thread # given. 0 (default) means log all"},
			{GDFLT, 0}
		};
		goptions.push_back(GOption(jsonTLOGOption));

		// event log every N events
		json jsonDigiVariationOption = {
			{GNAME, "digitizationVariation"},
			{GDESC, "Digitization Variation"},
			{GDFLT, "default"}
		};
		goptions.push_back(GOption(jsonDigiVariationOption));


		// event duration
		json jsonEventDurationOption = {
			{GNAME, "eventTimeSize"},
			{GDESC, "event duration with unit. Examples: 4*ns, 2*ms"},
			{GDFLT, "0*ns"}
		};
		goptions.push_back(GOption(jsonEventDurationOption));


		// other modules goptions
		goptions += gsystem::defineOptions();
		goptions += g4system::defineOptions();
		goptions += gstreamer::defineOptions();
		goptions += g4display::defineOptions();
		goptions += eventDispenser::defineOptions();

		return goptions;
	}


}



