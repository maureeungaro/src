// gemc
#include "gutsConventions.h"
#include "goptions.h"
#include "gemcOptions.h"

// options definitions
#include "eventDispenserOptions.h"
#include "g4displayOptions.h"
#include "g4systemOptions.h"
#include "gfieldOptions.h"
#include "gparticleOptions.h"
#include "gphysicsOptions.h"
#include "gstreamerOptions.h"
#include "gsystemOptions.h"

// c++
#include <iostream>

using namespace std;


namespace gemc {

    // returns array of options definitions
    GOptions defineOptions() {
        GOptions goptions;

        // switches
        goptions.defineSwitch("stream", "Activate Streaming ReadOut");
        goptions.defineSwitch("recordZeroEdep", "Record particle even if they do not deposit energy in the sensitive volumes");

        goptions.defineOption(GVariable("nthreads", 1, "sets number of threads."), "Default: 0 (use one thread for each available cores)");
        goptions.defineOption(GVariable("thread_log", 0, "Log only thread number"), "Default: 0 (log all threads)");
        goptions.defineOption(GVariable("em_log", 0, "Event Modulo log"), "Logs every <value> events. Default: 0 (log all events)");


        goptions.defineOption(GVariable("digitization_variation", "default", "digitization variation"), "Default: default");
        goptions.defineOption(GVariable("eventTimeSize", "0*ns", "event duration with unit"), "Default: 0*ns");


        // random engine name
        // the names come from the CLHEP library, can be found with
        // grep ": public HepRandomEngine" *.h $CLHEP_BASE_DIR/include/CLHEP/Random/* | awk -Fclass '{print $2}' | awk -F: '{print $1}'
        string help = "Random Engine Name: set the CLHEP:HepRandomEngine. Default: " + string(DEFAULT_RANDOM_ENGINE) + "\n";
        help += string(HELPFILLSPACE) + "The available CLHEP:HepRandomEngine are:\n\n";
        help += string(HELPFILLSPACE) + " - DRand48Engine\n";
        help += string(HELPFILLSPACE) + " - DualRand\n";
        help += string(HELPFILLSPACE) + " - Hurd160Engine\n";
        help += string(HELPFILLSPACE) + " - Hurd288Engine\n";
        help += string(HELPFILLSPACE) + " - HepJamesRandom\n";
        help += string(HELPFILLSPACE) + " - MTwistEngine\n";
        help += string(HELPFILLSPACE) + " - NonRandomEngine\n";
        help += string(HELPFILLSPACE) + " - RandEngine\n";
        help += string(HELPFILLSPACE) + " - RanecuEngine\n";
        help += string(HELPFILLSPACE) + " - Ranlux64Engine\n";
        help += string(HELPFILLSPACE) + " - RanluxEngine\n";
        help += string(HELPFILLSPACE) + " - RanluxppEngine final\n";
        help += string(HELPFILLSPACE) + " - RanshiEngine\n";
        help += string(HELPFILLSPACE) + " - TripleRand\n";
        goptions.defineOption(GVariable("randomEngineName", DEFAULT_RANDOM_ENGINE, "randomEngineName"), help);



        // random seed
        help = "Random Seed: set the random seed to an integer value. Default: -99\n";
        help += string(HELPFILLSPACE) + "If the random seed is set to -99, the seed will be set using: \n";
        help += string(HELPFILLSPACE) + " - local time \n";
        help += string(HELPFILLSPACE) + " - process id \n";
        help += string(HELPFILLSPACE) + " - clock function \n";
        goptions.defineOption(GVariable("randomSeed", -12345, "randomEngineName"), help);


        // load other
        goptions += eventDispenser::defineOptions();
        goptions += g4display::defineOptions();
        goptions += g4system::defineOptions();
        goptions += gfield::defineOptions();
        goptions += gparticle::defineOptions();
        goptions += gphysics::defineOptions();
        goptions += gstreamer::defineOptions();
        goptions += gsystem::defineOptions();

        return goptions;
    }


}
