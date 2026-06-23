#include <cstdio>
#include <cstdlib>

#include "HelloWorld.h"

namespace {

int run_phase(const char* name, HelloWorld& object, OStatus (HelloWorld::*phase)(const OSystemEvent&)) {
    const OSystemEvent event{};
    std::printf("[host-sim] %s\n", name);
    const OStatus status = (object.*phase)(event);
    std::printf("[host-sim] %s -> %d\n", name, status);
    return status == oSUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}

}  // namespace

int main() {
    HelloWorld object;

    if (run_phase("DoInit", object, &HelloWorld::DoInit) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (run_phase("DoStart", object, &HelloWorld::DoStart) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (run_phase("DoStop", object, &HelloWorld::DoStop) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (run_phase("DoDestroy", object, &HelloWorld::DoDestroy) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    std::printf("[host-sim] lifecycle completed\n");
    return EXIT_SUCCESS;
}
