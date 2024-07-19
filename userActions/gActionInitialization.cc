// gemc
#include "gActionInitialization.h"
#include "event/gEventAction.h"
#include "generator/gPrimaryGeneratorAction.h"
#include "run/gRunAction.h"
#include "gemcUtilities.h"
#include "gfactory.h"
#include "gstreamerOptions.h"

using namespace gstreamer;

// c++
#include <iostream>

using namespace std;

GActionInitialization::GActionInitialization(GOptions *gopts, map<string, GDynamicDigitization *> *gDDGlobal) :
        G4VUserActionInitialization(),
        GStateMessage(gopts, "GActionInitialization", "verbosity"),  // GStateMessage derived
        goptions(gopts),
        gDigitizationGlobalMap(gDDGlobal) {
    logSummary("Instantiating GActionInitialization ");

    string pluginPath = string(getenv("GEMC")) + "/lib/";


    // gstreamerFactory
    gstreamerFactoryMap = new map<string, GStreamer *>;

    // projecting options onto vector of JOutput
    vector<GStreamerDefinition> goutput_defs = getJOutputs(gopts);

    // if any output is specified, loading its factory
    if (goutput_defs.size() > 0) {

        GManager gStreamerManager("GOutput", verbosity);

        // the available plugins names are formatted as "xxxGMedia".
        for (auto &goutput_def: goutput_defs) {
            string factory = joutput.format;

            if (factory != UNINITIALIZEDSTRINGQUANTITY && outputFileName != UNINITIALIZEDSTRINGQUANTITY) {
                string pluginName = pluginPath + gstreamerPluginNameFromFactory(factory);
                string factoryMapKey = factory + "/" + streamType;

                if (gstreamerFactoryMap->find(factoryMapKey) == gstreamerFactoryMap->end()) {
                    (*gstreamerFactoryMap)[factoryMapKey] = gStreamerManager.LoadAndRegisterObjectFromLibrary<GStreamer>(pluginName);
                    (*gstreamerFactoryMap)[factoryMapKey]->define_ouput(goutput_def);
                    (*gstreamerFactoryMap)[factoryMapKey]->openConnection();
                }
            }
        }

        // done with gStreamerManager
        //	gStreamerManager.clearDLMap();

    }
}

// Destructor:
// delete gstreamerFactoryMap
GActionInitialization::~GActionInitialization() {
    // close output connections
    for (auto gsf: (*gstreamerFactoryMap)) {
        // protecting against DL failure
        if (gsf.second != nullptr) {
            gsf.second->closeConnection();
        }
    }
}

void GActionInitialization::Build() const {
    logDetail("GActionInitialization Thread Build ");

    SetUserAction(new GPrimaryGeneratorAction(goptions));
    SetUserAction(new GRunAction(goptions, gDigitizationGlobalMap, gstreamerFactoryMap));
    SetUserAction(new GEventAction(goptions));
}

void GActionInitialization::BuildForMaster() const {
    logSummary("GActionInitialization Master Build ");

    SetUserAction(new GRunAction(goptions, gDigitizationGlobalMap, gstreamerFactoryMap));
}
