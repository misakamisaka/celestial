#ifndef _CELESTIAL_SNAPSHOT_H_
#define _CELESTIAL_SNAPSHOT_H_

#include "snapshot.h"

namespace celestial {
class SnapshotManager {
public:
    void add(std::shared_ptr<Snapshot> snapshot);
};
}

#endif
