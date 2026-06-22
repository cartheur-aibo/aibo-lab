#ifndef HELLO_BODY_H_
#define HELLO_BODY_H_

// Placeholder header for an OPEN-R object.
// Replace base classes/includes with those from your installed Sony OPEN-R samples.

class HelloBody {
public:
    HelloBody();
    ~HelloBody();

    void DoInit();
    void DoStart();
    void DoStop();
    void DoDestroy();

    void OnSensorEvent();

private:
    void SetStartupExpression();
    void NodHeadOnce();
};

#endif
