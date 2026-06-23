#ifndef APERIOS_HOST_OPENR_OOBSERVER_H
#define APERIOS_HOST_OPENR_OOBSERVER_H

class OObserver {
public:
    void AssertReady() { ready_ = true; }
    void DeassertReady() { ready_ = false; }
    bool IsReady() const { return ready_; }

private:
    bool ready_ = false;
};

#endif
