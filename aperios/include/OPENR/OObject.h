#ifndef APERIOS_HOST_OPENR_OOBJECT_H
#define APERIOS_HOST_OPENR_OOBJECT_H

#include <cstddef>
#include <cstdint>

enum OStatus {
    oSUCCESS = 0,
    oFAIL = -1,
};

struct OSystemEvent {
    int selector = 0;
};

struct ONotifyEvent {
    int observer_index = 0;

    int ObsIndex() const { return observer_index; }
};

class OObject {
public:
    virtual ~OObject() = default;

    virtual OStatus DoInit(const OSystemEvent&) { return oSUCCESS; }
    virtual OStatus DoStart(const OSystemEvent&) { return oSUCCESS; }
    virtual OStatus DoStop(const OSystemEvent&) { return oSUCCESS; }
    virtual OStatus DoDestroy(const OSystemEvent&) { return oSUCCESS; }
};

#endif
