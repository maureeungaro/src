// goptions 
#include "goptions.h"
#include "goptionsConventions.h"
#include "gversion.h"

// gemc
#include "gutilities.h"

// c++
#include <iostream>

using namespace std;


// constructor:
// - load user defined options, add goptions options
// - assign internal options (gdebug, gstrict)
// - parse the base jcard plus all imported jcards
// - parse the command line options
// - get our own option
GOptions::GOptions(int argc, char *argv[], GOptions user_defined_options) {

    executableName = gutilities::getFileFromPath(argv[0]);

    cout << endl;

    goptions += user_defined_options.goptions;
    switches += user_defined_options.switches;

    // switches for all everyone
    defineSwitch("gui", "use Graphical User Interface"); // gui mode

    // parsing command line to check for help
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0) {
            print_version();
            print_help();
        } else if (strcmp(argv[i], "-hweb") == 0) {
            print_web_help();
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--v") == 0 || strcmp(argv[i], "-version") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "help") == 0) {
            print_option_or_switch(argv[i + 1]);
            exit(EXIT_SUCCESS);
        }
    }

    // parsing command line to check if any switch is turned on
    for (int i = 1; i < argc; i++) {
        string candidateSwitch = string(argv[i]);
        if (candidateSwitch[0] == '-') {
            for (auto &[switchName, swiitchValue]: switches) {
                string candidateRoot = candidateSwitch.substr(1, candidateSwitch.size() - 1);

                if (switchName == candidateRoot) {
                    swiitchValue.turnOn();
                }
            }
        }
    }

    // finds the yamls
    vector <string> yaml_files = find_yaml(argc, argv);


    // parse the yaml files

    // parse command lines

}

void GOptions::print_option_or_switch(string tag) {
    if (switches.find(tag) != switches.end()) {
        cout << KGRN << "-" << tag << RST << ": " << switches[tag].getDescription() << endl << endl;
        cout << TPOINTITEM << "Default value is " << (switches[tag].getStatus() ? "on" : "off") << endl << endl;
    } else {
        string alt_tag = "+" + tag;
        for (auto &goption: goptions) {
            if (goption.name == tag || goption.name == alt_tag) {
                goption.print_help(true);
                exit(EXIT_SUCCESS);
            }
        }
        cerr << FATALERRORL << "the " << YELLOWHHL << tag << RSTHHR << " option is not known to this system. " << endl;
        gexit(EC__NOOPTIONFOUND);
    }
}


// Finds the (first) configuration file (yaml).
vector <string> GOptions::find_yaml(int argc, char *argv[]) {
    vector <string> yaml_files;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        size_t pos = arg.find(".yaml");
        if (pos != string::npos) yaml_files.push_back(arg);
    }

    if (yaml_files.size() == 0) {
        return {UNINITIALIZEDSTRINGQUANTITY};
    }

    return yaml_files;

}

// add a command line switch to the map of switches
void GOptions::defineSwitch(string name, string description) {
    if (switches.find(name) == switches.end()) {
        switches[name] = GSwitch(description);
    } else {
        std::cerr << FATALERRORL << "the " << YELLOWHHL << name << RSTHHR << " switch is already present." << std::endl;
        gexit(EC__DEFINED_SWITCHALREADYPRESENT);
    }
}

// checks if the option exists
bool GOptions::does_option_exist(string tag) {
    for (auto &goption: goptions) {
        if (goption.name == tag) {
            return true;
        }
    }
    return false;
}

// add a simple option to the map of options
void GOptions::defineOption(GVariable gvar, string help) {

    if (does_option_exist(gvar.name)) {
        std::cerr << FATALERRORL << "the " << YELLOWHHL << gvar.name << RSTHHR << " option is already present." << std::endl;
        gexit(EC__DEFINED_OPTION_ALREADY_PRESENT);
    } else {
        goptions.push_back(GOption(gvar, help));
    }
}


// add a map option to the map of options
void GOptions::defineOption(string name, string description, vector <GVariable> gvars, string help) {

    if (does_option_exist(name)) {
        std::cerr << FATALERRORL << "the " << YELLOWHHL << name << RSTHHR << " option is already present." << std::endl;
        gexit(EC__DEFINED_OPTION_ALREADY_PRESENT);
    } else {
        goptions.push_back(GOption(name, description, gvars, help));
    }

}


// print only the non default settings set by users
void GOptions::print_settings(bool withDefaults) {
//    // making sure at least one option has value
//    bool canPrint = false;
//
//    for (auto &jOption: goptions) {
//        if (jOption.jOptionAssignedValues.size()) {
//            canPrint = true;
//        }
//    }
//
//    // nothing to do.
//    if (!canPrint) {
//        cout << KGRN << " No settings defined. " << RST << endl;
//        return;
//    }
//
//    if (withDefaults) {
//        cout << endl << KGRN << " All Settings: " << RST << endl << endl;
//    } else {
//        cout << endl << KGRN << " Non Default User Settings: " << RST << endl << endl;
//    }
//    // switches
//    for (auto [name, switchValue]: switches) {
//        if (withDefaults) {
//            // print all switches
//            cout << KGRN << ARROWITEM << name << RST << ": " << (switchValue.getStatus() ? "on" : "off") << endl;
//        } else {
//            // only print the active switches
//            if (switchValue.getStatus()) {
//                cout << KGRN << ARROWITEM << name << RST << ": on" << endl;
//            }
//        }
//    }
//
//    for (auto &jOption: goptions) {
//
//        jOption.printOption(withDefaults);
//
//        // non structured option, the jOptionAssignedValues has only one object, the json size is 1
//        if (jOption.jOptionAssignedValues.size() == 1 && jOption.jOptionAssignedValues.front().size() == 1) {
//        } else {
//            cout << endl;
//        }
//    }
    cout << endl;
}




//int GOptions::getInt(string tag) {
//    json jn = getNonStructuredOptionSingleValue(tag); // will exit if not found
//    return jn[tag].get<int>();
//}
//
//float GOptions::getFloat(string tag) {
//    json jn = getNonStructuredOptionSingleValue(tag); // will exit if not found
//    return jn[tag].get<float>();
//}
//
//double GOptions::getDouble(string tag) {
//    json jn = getNonStructuredOptionSingleValue(tag); // will exit if not found
//    return jn[tag].get<double>();
//}
//
//string GOptions::getString(string tag) {
//    json jn = getNonStructuredOptionSingleValue(tag); // will exit if not found
//    return jn[tag].get<string>();
//}
//
//bool GOptions::getSwitch(string tag) {
//    if (switches.find(tag) != switches.end()) {
//        return switches[tag].getStatus();
//    } else {
//        cerr << FATALERRORL << "the " << YELLOWHHL << tag << RSTHHR << " switch is not known to this system. " << endl;
//        gexit(EC__NOOPTIONFOUND);
//    }
//    return false;
//}





// overloaded operator to add option vectors
vector <GOption> &operator+=(vector <GOption> &original, vector <GOption> optionsToAdd) {

    for (const auto &optionToadd: optionsToAdd) {
        original.push_back(optionToadd);
    }

    return original;
}


map <string, GSwitch> &operator+=(map <string, GSwitch> &original, map <string, GSwitch> optionsToAdd) {

    for (const auto &[name, switchToAdd]: optionsToAdd) {
        original[name] = switchToAdd;
    }

    return original;
}


// print only the non default settings set by users
void GOptions::print_help() {

    long int fill_width = string(HELPFILLSPACE).size() + 1;
    cout.fill('.');

    cout << KGRN << " Usage: " << RST << endl << endl;

    // switches help, belongs here cause of the map key
    for (auto &s: switches) {
        string help = "-" + s.first + RST + " ";
        cout << KGRN << " " << left;
        cout.width(fill_width);
        cout << help;
        cout << ": " << s.second.getDescription() << endl;
    }

    for (auto &option: goptions) {
        option.print_help(false);
    }

    cout << endl;

    // add the help, web help, introspection, single help and search
    vector <string> helps = {
            string("-h, --h, -help, --help") + RST,
            string("print this help and exit"),
            string("-hweb") + RST,
            string("print this help in web format"),
            string("-v, --v, -version, --version") + RST,
            string("print the version\n"),
            string("help <value>") + RST,
            string("print detailed help for option <value>"),
            string("search <value>") + RST,
            string("list all options containing <value> in the description\n")
    };

    for (auto i = 0; i < helps.size() / 2; i++) {
        cout << KGRN << " " << left;
        cout.width(fill_width);
        cout << helps[i * 2] << ": " << helps[i * 2 + 1] << endl;
    }
    cout << endl;

    cout << " Note: command line options overwrite yaml file(s). " << endl << endl;

    exit(EXIT_SUCCESS);
}


// print only the non default settings set by users
void GOptions::print_web_help() {

    long int fill_width = string(HELPFILLSPACE).size() + 1;
    cout.fill('.');

    cout << KRED << " Usage: " << RST << endl << endl;


    exit(EXIT_SUCCESS);
}

// introspection
void GOptions::print_version() {
    string asterisks = "**************************************************************";
    cout << endl << asterisks << endl;
    cout << " " << KRED << KBOLD << executableName << RST << "  version: " << KGRN << gversion << RST << endl;
    cout << " Released on: " << KGRN << grelease_date << RST << endl;
    cout << " Reference: " << KGRN << greference << RST << endl;
    cout << " Homepage: " << KGRN << gweb << RST << endl;
    cout << " Author: " << KGRN << gauthor << RST << endl << endl;
    cout << asterisks << endl << endl;
}
