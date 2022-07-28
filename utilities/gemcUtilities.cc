// Qt
#include <QtWidgets>

// geant4 headers
#include "G4Threading.hh"
#include "G4UImanager.hh"

// c++
#include <iostream>
using namespace std;

// glibrary
#include "goptions.h"

// conventions, utilities, options definitions
#include "gemcConventions.h"
#include "gemcUtilities.h"

// distinguishing between graphical and batch mode
QCoreApplication* createQtApplication(int &argc, char *argv[], bool gui)
{
	if( gui ) {
		QFileInfo qrcFileInfoExecutable(argv[0]);
		QString rccPath = qrcFileInfoExecutable.absolutePath() + "/" + "qtresources.rcc";
		QResource::registerResource(rccPath);

		return new QApplication(argc, argv);
	} else {
		return new QCoreApplication(argc, argv);
	}
}

// return number of cores from options. If 0 or none given,
// returns max number of available cores
int getNumberOfThreads(GOptions* gopts) {
	int useThreads = gopts->getInt("nthreads");
	int allThreads = G4Threading::G4GetNumberOfCores();
	if(useThreads == 0) useThreads = allThreads;

	int verbosity = gopts->getInt("verbosity");

	// global log screen
	if (verbosity >= GVERBOSITY_SUMMARY) {
		cout << GEMCLOGMSGITEM << "G4MTRunManager: using " << useThreads << " threads out of "  << allThreads << " available."  << endl;
	}
	
	return useThreads;
}


// initialize G4MTRunManager
void initGemcG4RunManager(G4RunManager *grm, GOptions* gopts)
{
	int tlog = gopts->getInt("tlog");

	G4UImanager *g4uim   = G4UImanager::GetUIpointer();
	g4uim->ApplyCommand("/control/cout/setCoutFile gthread.log");
	g4uim->ApplyCommand("/control/cout/ignoreThreadsExcept " + to_string(tlog));

	// done in event dispenser
	grm->Initialize();

}


vector<string> startingUIMCommands(bool gui, int checkForOverlaps) {
	vector<string> commands;

	// define batch commands

//	commands.push_back("/process/verbose 0");
//	commands.push_back("/run/verbose 2");

//	commands.push_back("/particle/process/verbose 0 -1");
//	commands.push_back("/process/setVerbose 0 all");
//	commands.push_back("/tracking/verbose -1");
//	commands.push_back("/particle/verbose 0");
//	commands.push_back("/particle/property/verbose 0");
//	commands.push_back("/process/had/rdm/verbose 0");
//	commands.push_back("/process/had/verbose 0");
//	commands.push_back("/event/verbose 2");
//	commands.push_back("/cuts/verbose 0");
//	commands.push_back("/run/verbose 0");
//	commands.push_back("/run/particle/verbose 0");
//	commands.push_back("/process/eLoss/verbose 0");
//	commands.push_back("/process/em/verbose 0");
//	commands.push_back("/process/em/workerVerbose 0");
//	commands.push_back("/material/verbose 0");
//	commands.push_back("/process/had/deex/verbose 0");
//	commands.push_back("/hits/verbose 0");
//	commands.push_back("/control/verbose 0");
//	commands.push_back("/geometry/navigator/verbose 0");
//	commands.push_back("/vis/verbose 0");
//	commands.push_back("/vis/viewer/flush");

	
	// added additional overlaps check if set to 2
	// if set to a number greater than 100
	if ( checkForOverlaps == 2 ) {
		cout << GEMCLOGMSGITEM << "Running /geometry/test/run with 50 points. NOTICE: currently this fails" << endl;
		commands.push_back("/geometry/test/resolution 50" );
		commands.push_back("/geometry/test/run");
	} else if ( checkForOverlaps >= 100 ) {
		cout << GEMCLOGMSGITEM << "Running /geometry/test/run with " << to_string(checkForOverlaps) << " points. NOTICE: currently this fails. " << endl;
		commands.push_back("/geometry/test/resolution " + to_string(checkForOverlaps));
		commands.push_back("/geometry/test/run");
	}
	
	
	// incomplete list, see $G4INSTALL/lib $G4INSTALL/source/geant4-v$GEANT4_VERSION/examples/basic/B1/*.mac
//	commands.push_back("/vis/open DAWNFILE");
//	commands.push_back("/vis/drawVolume");
//	commands.push_back("/vis/viewer/set/viewpointVector -0.5 0 -1");
//	commands.push_back("/vis/viewer/set/lightsVector -1 0 0");
//	commands.push_back("/vis/scene/endOfEventAction accumulate");
//	commands.push_back("/vis/geometry/set/visibility World 0 false");
//	commands.push_back("/vis/viewer/set/style surface");

	
	
	
	
	// not in gui mode, return batch only
	if( !gui ) return commands;

	// define gui commands
	commands.push_back("/vis/scene/add/trajectories rich smooth");
	commands.push_back("/vis/scene/add/hits");
	commands.push_back("/vis/scene/endOfEventAction accumulate");        // for some reason refresh (default) won't work here
	commands.push_back("/vis/viewer/set/culling coveredDaughters true");
	commands.push_back("/vis/viewer/set/background 1 1 1 1");   // 205, 230, 251 = CD, E6, FA


	commands.push_back("/vis/viewer/set/autoRefresh true");
	commands.push_back("/vis/drawVolume");
	commands.push_back("/vis/viewer/set/lineSegmentsPerCircle 100");

	
	
	
	return commands;

}

// apply initial UIM commands coming from, in order:
// - batch
// - gui (if needed)
// - goptions
void applyInitialUIManagerCommands(bool gui, int checkForOverlaps, int verbosity) {
	G4UImanager *g4uim = G4UImanager::GetUIpointer();

	vector<string> commands = startingUIMCommands(gui, checkForOverlaps);

	for(auto &c : commands) {
		if(verbosity > GVERBOSITY_SUMMARY) {
			cout << GEMCLOGMSGITEM << "Executing UIManager command \"" << c << "\"" << endl;
		}
		g4uim->ApplyCommand(c.c_str());
	}
}


string definePluginPath(GOptions* gopts) {
	// the plugin is loaded from the GPLUGIN_PATH environment variable
	// however if gpluginsPath is defined in the jcard, it will overwrite the plugin location
	auto pluginPathENV = getenv("GPLUGIN_PATH"); // char*
	string pluginPathOption = gopts->getString("gpluginsPath");

	string pluginPath = UNINITIALIZEDSTRINGQUANTITY;

	if ( pluginPathENV != nullptr ) {
		pluginPath = string(pluginPathENV) + "/";
	}
	if ( pluginPathOption != UNINITIALIZEDSTRINGQUANTITY ) {
		pluginPath = pluginPathOption  + "/";
	}
	// set to current dir if pluginPath is still not defined
	if ( pluginPath == UNINITIALIZEDSTRINGQUANTITY ) {
		pluginPath = "./";
	}

	return pluginPath;
}
