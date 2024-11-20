#include <logorithm/Logger.h>

using enum LOGLEVEL;

int main() {
    logger.log("This is an info message.", Info);
    logger.log("This is a warning message.", Warning);
    logger.log("This is an error message.", Error);
    return 0;
}
