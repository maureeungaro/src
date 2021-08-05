// c++
#include <iostream>
using namespace std;

// glibrary
#include "goptions.h"
#include "gsplash.h"

// utilities, conventions, options definitions
#include "gemcUtilities.h"
#include "gemcConventions.h"
#include "gemcOptions.h"

// detector
#include "gdetectorConstruction.h"

// gsession
#include "gsession.h"

// userActions
#include "gActionInitialization.h"


// geant4
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4MTRunManager.hh"

// TODO: physics list: to be gphysics
#include "FTFP_BERT.hh"


int main(int argc, char* argv[])
{
	// the gemc goptions are defined in utilities/defineOptions.cc
	// they are merged with the frameworks goptions definitions
	// the goptions are then assigned from the jcard(s) and command line
	GOptions *gopts = new GOptions(argc, argv, gemc::defineOptions());
	gopts->printSettings(true);

	// splash screen
	GSplash  *gemcSplash = nullptr;

	// get gui switch
	bool gui = gopts->getSwitch("gui");

	// createQtApplication returns a QApplication if gui is not zero
	// otherwise it returns a QCoreApplication
	QCoreApplication *gApp = createQtApplication(argc, argv, gui);
	if ( gui ) {
		gemcSplash = new GSplash("gemcArchitecture");
	}

	// instantiating new User Interface Messenger
	// our custom cout destination for the UIM: MasterGeant4.[log, err]
	G4UImanager* UIM = G4UImanager::GetUIpointer();
	UIM->SetCoutDestination(new GSession);

	// init geant4 run manager with number of threads coming from options
	G4MTRunManager *g4MTRunManager = new G4MTRunManager;
	g4MTRunManager->SetNumberOfThreads(getNumberOfThreads(gopts));

	// instantiating pointer to global digitization map
	// the map will be filled with the gsystem information of the sensitive detectors
	map<string, GDynamicDigitization*> *globalDigitizationMap = new map<string, GDynamicDigitization*>;

	// building detector
	// this is global, changed at main scope
	GDetectorConstruction *gDetectorGlobal = new GDetectorConstruction(gopts, globalDigitizationMap);
	g4MTRunManager->SetUserInitialization(gDetectorGlobal);

	// TODO: physics list: to be gphysics
	auto physicsList = new FTFP_BERT;
	g4MTRunManager->SetUserInitialization(physicsList);


	// instantiate GActionInitialization and initialize the geant4 kernel
	g4MTRunManager->SetUserInitialization(new GActionInitialization(gopts, globalDigitizationMap));

	// this Initialize g4MTRunManager, which:
	// calls Construct in GDetectorConstruction
	// calls ConstructSDandField in GDetectorConstruction
	initGemcG4RunManager(g4MTRunManager, gopts);




	// order of pointers deletion is inverse of creation
	delete gDetectorGlobal;
	delete globalDigitizationMap;

	// TODO: apparently can't delete this yet
	//delete g4MTRunManager;

	if ( gui ) {





		delete UIM;
		delete gemcSplash;
	}
	delete gApp;
	delete gopts;
	
	cout << GEMCLOGMSGITEM << " Simulation completed, arrivederci! " << endl << endl;
	return EXIT_SUCCESS;
}



