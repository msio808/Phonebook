#include "../includes/setup.h"
#include "../includes/utilities.h"

int main() {
    clr();
    if (!init_default()) {
        return EXIT_FAILURE;
    }
    login();
    return EXIT_SUCCESS;
}
