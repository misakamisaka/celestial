#ifndef _CELESTIAL_STATE_MACHINE_H_
#define _CELESTIAL_STATE_MACHINE_H_

namespace celestial {
class StateMachine : public runnable {
public:
    virtual std::shared_ptr<Snapshot> takeSnapshot() = 0;
    virtual void loadSnapshot(std::shared_ptr<Snapshot>) = 0;
    virtual void apply(shared_ptr<Entry> entry) = 0;
    virtual void writeToStream(std::ostream& os) = 0;
    virtual void readFromStream(std::istream& is) = 0;
};
}

#endif
