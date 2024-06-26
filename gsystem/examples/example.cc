#ifndef DOXYGEN_SHOULD_SKIP_THIS

// gsystem
#include "gworld.h"

// goptions
#include "goptions.h"

// gstrings
#include "gutilities.h"
using namespace gutilities;

// c++
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	GOptions *gopts = new GOptions(argc, argv, gsystem::defineOptions());

	GWorld *world = new GWorld(gopts);
	delete world;
	
	return EXIT_SUCCESS;
}


#endif
