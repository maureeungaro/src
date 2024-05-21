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
// - parse the yaml files
// - parse the command line options
GOptions::GOptions(int argc, char *argv[], GOptions user_defined_options) {

    executableName = gutilities::getFileFromPath(argv[0]);

    cout << endl;

    goptions += user_defined_options.goptions;
    switches += user_defined_options.switches;

    // switches for all everyone
    defineSwitch("gui", "use Graphical User Interface");
    defineOption(GVariable("conf_yaml", "saved_configuration", "the yaml filename prefix where all used options are saved"),
                 "The default value appends \"_saved_configuration\" to the executable name.");

    // version is a special option, not settable by the user
    // it is set by the gversion.h file
    // we add it here so it can be saved to the yaml file
    vector <GVariable> version = {
            {"release",      gversion,      "release version number"},
            {"release_date", grelease_date, "release date"},
            {"Reference",    greference,    "article reference"},
            {"Homepage",     gweb,          "homepage"},
            {"Author",       gauthor,       "author"}
    };
    defineOption(GVERSION_STRING, "version information", version, "Version information. Not settable by user.");


    vector <GVariable> verbosity = {
            {"fields",          0, "verbosity for fields"},
            {"particles",       0, "verbosity for particles"},
            {"hits",            0, "verbosity for hits"},
            {"detectors",       0, "verbosity for detectors"},
            {"materials",       0, "verbosity for materials"},
            {"regions",         0, "verbosity for regions"},
            {"volumes",         0, "verbosity for volumes"},
            {"surfaces",        0, "verbosity for surfaces"},
            {"optical",         0, "verbosity for optical properties"},
            {"event_dispenser", 0, "verbosity for the event dispenser"},
            {"g4display",       0, "verbosity for the g4display"},
    };

    string help = "Effects: \n \n";
    help += "0: shush\n";
    help += "1: summaryt\n";
    help += "2: details\n";
    help += "3: everything\n \n";
    help += "Example: -verbosity=\"[{fields: 3}, {materials: 1}]\" \n";
    defineOption("verbosity", "Sets the log verbosity for various categories", verbosity, help);


    // parsing command line to check for help
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
        } else if (strcmp(argv[i], "-hweb") == 0) {
            print_web_help();
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--v") == 0 || strcmp(argv[i], "-version") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "help") == 0) {
            print_option_or_switch_help(argv[i + 1]);
            exit(EXIT_SUCCESS);
        }
    }


    // finds the yaml files
    vector <string> yaml_files = find_yamls(argc, argv);

    // parse the yaml files
    for (auto &yaml_file: yaml_files) {
        cout << "Parsing " << yaml_file << endl;
        set_options_values_from_yaml_file(yaml_file);
    }


    // parse command lines
    // check that every option passed is either a switch, an option or a yaml file
    for (int i = 1; i < argc; i++) {
        string candidate = string(argv[i]);
        // empty string
        if (candidate == "") continue;

        if (find(yaml_files.begin(), yaml_files.end(), candidate) == yaml_files.end()) {

            // if the command line is not a yaml file, check that is a valid
            // - switch: starts with a dash
            // - option: a dash followed by a string and an equal sign
            if (candidate[0] == '-') {

                // checking for a switch
                string possible_switch = candidate.substr(1, candidate.size() - 1);

                // switch found, turn it on
                if (switches.find(possible_switch) != switches.end()) {
                    switches[possible_switch].turnOn();
                } else {
                    // not a switch, check if it is an option
                    // checking if '-' is present
                    if (possible_switch.find("=") != string::npos) {

                        string possible_option = possible_switch.substr(0, candidate.find("=") - 1);

                        // option found, parse it
                        if (does_option_exist(possible_option)) {
                            string possible_yaml_node = possible_switch.substr(candidate.find("="), candidate.size() - 1);
                            set_option_values_from_command_line_argument(possible_option, possible_yaml_node);
                        } else {
                            // option not found
                            cerr << FATALERRORL << "the " << YELLOWHHL << candidate << RSTHHR << " option is not known to this system. " << endl;
                            cerr << endl << "   " << executableName << " -h for help." << endl << endl;
                            gexit(EC__NOOPTIONFOUND);

                        }
                    } else {
                        // not a switch, not an option
                        cerr << FATALERRORL << YELLOWHHL << candidate << RSTHHR << " is not a valid command line option or switch.  " << endl;
                        cerr << " Note: switches start with a dash; options start with a dash, and are followed by an equal sign and their desired value."
                             << endl;
                        cerr << endl << " Usage: " << endl << endl;
                        cerr << "   " << executableName << " [options] [yaml files]" << endl;
                        cerr << "   " << executableName << " -h for help." << endl << endl;
                        gexit(EC__NOOPTIONFOUND);
                    }
                }
            } else {
                // not a file, not a switch, not an option
                cerr << FATALERRORL << "the " << YELLOWHHL << candidate << RSTHHR << " command line is not known to this system. " << endl;
                cerr << endl << " Usage: " << endl << endl;
                cerr << "   " << executableName << " [options] [yaml files]" << endl;
                cerr << "   " << executableName << " -h for help." << endl << endl;
                gexit(EC__NOOPTIONFOUND);
            }
        }
    }

    // print version no matter what
    print_version();

    // save options to yaml
    string yaml_conf_filename = executableName + "." + getScalarString("conf_yaml") + ".yaml";
    cout << " Saving options to " << yaml_conf_filename << endl << endl;
    yaml_conf = ofstream(yaml_conf_filename);

    save_options();
}

void GOptions::print_option_or_switch_help(string tag) {
    if (switches.find(tag) != switches.end()) {
        cout << KGRN << "-" << tag << RST << ": " << switches[tag].getDescription() << endl << endl;
        cout << TPOINTITEM << "Default value is " << (switches[tag].getStatus() ? "on" : "off") << endl << endl;
        exit(EXIT_SUCCESS);
    } else {
        for (auto &goption: goptions) {
            if (goption.name == tag) {
                goption.print_help(true);
                exit(EXIT_SUCCESS);
            }
        }
        cerr << FATALERRORL << "the " << YELLOWHHL << tag << RSTHHR << " option is not known to this system. " << endl;
        gexit(EC__NOOPTIONFOUND);
    }
}


// Finds the (first) configuration file (yaml).
vector <string> GOptions::find_yamls(int argc, char *argv[]) {
    vector <string> yaml_files;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        size_t pos = arg.find(".yaml");
        if (pos != string::npos) yaml_files.push_back(arg);
    }

    return yaml_files;
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

// add a command line switch to the map of switches
void GOptions::defineSwitch(string name, string description) {
    if (switches.find(name) == switches.end()) {
        switches[name] = GSwitch(description);
    } else {
        std::cerr << FATALERRORL << "the " << YELLOWHHL << name << RSTHHR << " switch is already present." << std::endl;
        gexit(EC__DEFINED_SWITCHALREADYPRESENT);
    }
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
void GOptions::defineOption(string name, string description, vector <GVariable> g_vars, string help) {

    if (does_option_exist(name)) {
        std::cerr << FATALERRORL << "the " << YELLOWHHL << name << RSTHHR << " option is already present." << std::endl;
        gexit(EC__DEFINED_OPTION_ALREADY_PRESENT);
    } else {
        goptions.push_back(GOption(name, description, g_vars, help));
    }
}

void GOptions::set_options_values_from_yaml_file(string yaml) {

    YAML::Node config;
    try {
        config = YAML::LoadFile(yaml);

    } catch (YAML::ParserException &e) {
        cerr << FATALERRORL << "Error parsing " << YELLOWHHL << yaml << RSTHHR << " yaml file." << endl;
        cerr << e.what() << endl;
        cerr << "Try validating the yaml file with an online yaml validator, for example: https://www.yamllint.com" << endl;
        gexit(EC__YAML_PARSING_ERROR);
    }

    for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {

        string option_name = it->first.as<std::string>();

        auto option_it = get_option_iterator(option_name);

        if (option_it == goptions.end()) {
            if (switches.find(option_name) == switches.end()) {
                cerr << FATALERRORL << "The option or switch " << YELLOWHHL << option_name << RSTHHR << " is not known to this system." << endl;
                gexit(EC__NOOPTIONFOUND);
            } else {
                switches[option_name].turnOn();
            }
        } else {
            switch (it->second.Type()) {
                case YAML::NodeType::Scalar:
                    option_it->set_scalar_value(it->second.as<std::string>());
                    break;
                case YAML::NodeType::Sequence:
                    option_it->set_value(it->second);
                    break;
                case YAML::NodeType::Map:
                    option_it->set_value(it->second);
                    break;
                default:
                    break;

            }
        }
    }
}

// parse a command line
void GOptions::set_option_values_from_command_line_argument(string option_name, string possible_yaml_node) {
    YAML::Node node = YAML::Load(possible_yaml_node);

    auto option_it = get_option_iterator(option_name);

    if (node.Type() == YAML::NodeType::Scalar) {
        option_it->set_scalar_value(possible_yaml_node);
    } else {
        option_it->set_value(node);
    }
}

// returns vector<GOption> iterator for option name
vector<GOption>::iterator GOptions::get_option_iterator(string name) {

    for (auto it = goptions.begin(); it != goptions.end(); ++it) {
        if (it->name == name) {
            return it;
        }
    }

    return goptions.end();
}

int GOptions::getScalarInt(string tag) {
    auto it = get_option_iterator(tag);
    return it->value.begin()->second.as<int>();
}

float GOptions::getScalarFloat(string tag) {
    auto it = get_option_iterator(tag);
    return it->value.begin()->second.as<float>();
}

double GOptions::getScalarDouble(string tag) {
    auto it = get_option_iterator(tag);
    return it->value.begin()->second.as<double>();
}

string GOptions::getScalarString(string tag) {
    auto it = get_option_iterator(tag);
    return it->value.begin()->second.as<string>();
}

// overloaded operator to add option vectors
vector <GOption> &operator+=(vector <GOption> &original, vector <GOption> optionsToAdd) {
    for (const auto &option_to_add: optionsToAdd) {
        original.push_back(option_to_add);
    }
    return original;
}

int GOptions::getVerbosityFor(string tag) {

    // this is the iterator pointing to the verbosity sequence
    auto verb_node = get_option_node("verbosity");

    for (auto seq_item: verb_node) {
        for (auto map_item = seq_item.begin(); map_item != seq_item.end(); ++map_item) {
            if (map_item->first.as<string>() == tag) {
                return map_item->second.as<int>();
            }
        }
    }

    cerr << FATALERRORL << "The verbosity for " << YELLOWHHL << tag << RSTHHR << " was not found." << endl;
    gexit(EC__NOOPTIONFOUND);

    return -1;
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

    cout << KRED << KBOLD << " " << executableName << RST << " [options] [yaml files]" << endl << endl;
    cout << " Switches: " << endl << endl;

    // switches help, belongs here cause of the map key
    for (auto &s: switches) {
        string help = "-" + s.first + RST + " ";
        cout << KGRN << " " << left;
        cout.width(fill_width);
        cout << help;
        cout << ": " << s.second.getDescription() << endl;
    }
    cout << endl;

    cout << " Options: " << endl << endl;

    for (auto &option: goptions) {
        option.print_help(false);
    }

    cout << endl;

    cout << endl << " Help / Search / Introspection: " << endl << endl;

    vector <string> helps = {
            string("-h, --h, -help, --help") + RST,
            string("print this help and exit"),
            string("-hweb") + RST,
            string("print this help in web format and exit"),
            string("-v, --v, -version, --version") + RST,
            string("print the version and exit\n"), string("help <value>") + RST,
            string("print detailed help for option <value> and exit"),
            string("search <value>") + RST,
            string("list all options containing <value> in the description and exit\n")
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

    exit(EXIT_SUCCESS);
}


// print options and switches values

void GOptions::save_options() {

    for (auto &s: switches) {
        string status = "false";
        if (s.second.getStatus()) status = "true";
        yaml_conf << s.first + ": " + status << "," << endl;
    }

    for (auto &option: goptions) {
        option.save_option(yaml_conf);
    }

    yaml_conf.close();
}

// introspection, add file option
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
