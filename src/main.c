#include "cgl.h"

int main() {
    if (cgl_init()) {
        cgl_run();
        cgl_cleanup();
        return 0;
    }
    return 1;
}
