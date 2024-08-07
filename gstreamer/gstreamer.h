#ifndef  GSTREAMER_H
#define  GSTREAMER_H  1

// glibrary
#include "event/gEventDataCollection.h"
#include "frame/gFrameDataCollection.h"
#include "gfactory.h"
#include "goptions.h"

// c++
#include <string>

// utility struct to load GStreamer from options
struct GStreamerDefinition {

    // default constructor
    GStreamerDefinition() = default;

    GStreamerDefinition(string f, string n, string t) : format(f), name(n), type(t) {}

    string format;
    string name;
    string type;

    string gstreamerPluginName() {
        return "gstreamer" + format + "Factory";
    }
};

class GStreamer {

public:
    virtual ~GStreamer() = default;

    virtual bool openConnection() { return false; }   // in GActionInitialization constructor
    virtual bool closeConnection() { return false; }  // in GActionInitialization destructor

    // called in GRunAction::EndOfRunAction
    // runs the protected virtual methods below to write events from a run to file
    // the key is the routine name + sensitive detector name
    map<string, bool> publishEventRunData(const GOptions *gopts, const vector<GEventDataCollection *> runData);

    // called in GRunAction::EndOfRunAction
    // runs the protected virtual methods below to write frames from a run to file
    // the key is the routine name + frame streamer id
    map<string, bool> publishFrameRunData(const GOptions *gopts, const GFrameDataCollection *frameRunData);

    inline string const getStreamType() const { return gstreamer_definitions.type; }
    inline void define_gstreamer(GStreamerDefinition gstreamerDefinition) { gstreamer_definitions = gstreamerDefinition; }

    static const vector<string> supported_formats ;
    static bool is_valid_format(string format);

protected:

    GStreamerDefinition gstreamer_definitions;

    // event virtual methods called by publishRunData, in order
    // --------------------------------------------------------

    virtual bool startEvent([[maybe_unused]] const GEventDataCollection *eventData) { return false; }

    virtual bool publishEventHeader([[maybe_unused]] const GEventDataCollectionHeader *gheader) { return false; }

    // vector index is hit number
    virtual bool
    publishEventTrueInfoData([[maybe_unused]] const string detectorName, [[maybe_unused]] const vector<GTrueInfoData *> *trueInfoData) { return false; }

    virtual bool
    publishEventDigitizedData([[maybe_unused]] const string detectorName, [[maybe_unused]] const vector<GDigitizedData *> *digitizedData) { return false; }

    virtual bool endEvent([[maybe_unused]] const GEventDataCollection *eventData) { return false; }

    // stream virtual methods
    virtual bool startStream([[maybe_unused]] const GFrameDataCollection *frameRunData) { return false; }

    virtual bool publishFrameHeader([[maybe_unused]] const GFrameDataCollectionHeader *gframeHeader) { return false; }

    virtual bool publishPayload([[maybe_unused]] const vector<GIntegralPayload *> *payload) { return false; }

    virtual bool endStream([[maybe_unused]] const GFrameDataCollection *frameRunData) { return false; }


public:

    // method to dynamically load factories
    static GStreamer *instantiate(const dlhandle handle) {

        if (handle == nullptr) return nullptr;

        // must match the extern C declaration in the derived factories
        void *maker = dlsym(handle, "GStreamerFactory");

        if (maker == nullptr) return nullptr;

        typedef GStreamer *(*fptr)();

        // static_cast not allowed here
        // see http://stackoverflow.com/questions/573294/when-to-use-reinterpret-cast
        // need to run the DLL GMediaFactory function that returns the factory
        fptr func = reinterpret_cast<fptr>(reinterpret_cast<void *>(maker));

        return func();
    }


};


#endif
