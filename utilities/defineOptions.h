#ifndef DEFINEOPTIONS_H
#define DEFINEOPTIONS_H 1

// goptions
#include "goptions.h"

namespace goptions {

//	// single option to activate gui
//	// -----------------------------
//
//	struct GemcConfiguration {
//		int gui;
//		int nthreads;
//		int stageMessageVerbosity;
//	};
//
//
//	void from_json(const json& j, GemcConfiguration& c);
//
//	// get the configuration from options
//	GemcConfiguration getGemcConfiguuration(GOptions *gopts);

}

// returns array of options definitions
// this calls all the static defineOptions functions in the various libraries/frameworks
vector<GOption> defineOptions();

#endif
