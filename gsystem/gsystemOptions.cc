// gsystem
#include "gsystemOptions.h"

// project goption to a system
namespace gsystem {


    // method to return a vector of GSystem from the options
    vector <GSystem> getSystems(GOptions *gopts) {

        vector <GSystem> systems;
        int verbosity = gopts->getVerbosityFor("gsystem");

        auto gsystem_node = gopts->get_option_node("gsystem");

        for (auto gsystem_item: gsystem_node) {
            systems.push_back(GSystem(
                    gopts->get_variable_in_option<string>(gsystem_item, "name", goptions::NODFLT),
                    gopts->get_variable_in_option<string>(gsystem_item, "factory", "TEXT"),
                    gopts->get_variable_in_option<string>(gsystem_item, "variation", "default"),
                    verbosity,
                    gopts->get_variable_in_option<int>(gsystem_item, "runno", 1),
                    gopts->get_variable_in_option<string>(gsystem_item, "annotations", UNINITIALIZEDSTRINGQUANTITY),
                    gopts->get_variable_in_option<string>(gsystem_item, "sqlite_file", GSYSTEMSQLITETDEFAULTFILE)
            ));
        }

        return systems;
    }


    // method to return a vector of GModifier from the options
    vector <GModifier> getModifiers(GOptions *gopts) {

        vector <GModifier> gmods;
        int verbosity = gopts->getVerbosityFor("gsystem");

        auto gmodifier_node = gopts->get_option_node("gmodifier");

        for (auto gmodifier_item: gmodifier_node) {

            gmods.push_back(GModifier(
                    gopts->get_variable_in_option<string>(gmodifier_item, "name", goptions::NODFLT),
                    gopts->get_variable_in_option<string>(gmodifier_item, "shift", GSYSTEMNOMODIFIER),
                    gopts->get_variable_in_option<string>(gmodifier_item, "tilt", GSYSTEMNOMODIFIER),
                    gopts->get_variable_in_option<bool>(gmodifier_item, "isPresent", true),
                    verbosity
            ));
        }

        return gmods;
    }


    // returns array of options definitions
    GOptions defineOptions() {
        GOptions goptions;

        // System
        string help;
        help = "A system definition includes the geometry location, factory and variation \n \n";
        help += "Example: +gsystem={detector: \"experiments/clas12/targets\", factory: \"TEXT\", variation: \"bonus\"}";

        vector <GVariable> gsystem = {
                {"name",      goptions::NODFLT,              "system name (mandatory). For TEXT factories, it may include the path to the file"},
                {"factory",   "TEXT",                        "factory name. Possible choices: TEXT, CAD, GDML, SQLITE. Default is TEXT"},
                {"variation", "default",                     "geometry variation, default is \"default\")"},
                {"runno",     1,                             "runno, default is 1)"},
                {"annotations", UNINITIALIZEDSTRINGQUANTITY, "optional system annotations. Examples: \"mats_only\" "},
                {"sqlite_file", GSYSTEMSQLITETDEFAULTFILE,   "sqlite file name, default is " + string(GSYSTEMSQLITETDEFAULTFILE)}
        };
        goptions.defineOption("gsystem", "defines the group of volumes in a system", gsystem, help);

        // Modifier
        help = "The volume modifer can shift, tilt, or delete a volume from the gworld \n \n";
        help += "Example: +gmodifier={volume: \"targetCell\", tilt: \"0*deg, 0*deg, -10*deg\" }";

        vector <GVariable> gmodifier = {
                {"name",      goptions::NODFLT, "volume name (optional)"},
                {"shift", GSYSTEMNOMODIFIER,    "volume shift added to existing position"},
                {"tilt",  GSYSTEMNOMODIFIER,    "volume tilt added to existing rotation"},
                {"isPresent", true,             "f set to false, remove volume from world i"}
        };
        goptions.defineOption("gmodifier", "modify volume existence or placement", gmodifier, help);

        goptions.defineOption(GVariable(ROOTWORLDGVOLUMENAME, ROOTDEFINITION, "root volume definition"), "root volume definition");

        return goptions;
    }
}
