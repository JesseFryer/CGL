#include "cgl.h"

int main(void) {
    if (cgl_init()) {
        cgl_run();
        cgl_cleanup();
        return 0;
    }
    return 1;
}
