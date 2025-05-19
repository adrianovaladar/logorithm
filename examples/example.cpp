#include <logorithm/Logger.h>

using enum LOGLEVEL;

int main() {
    /* Uncomment the next line to filter logs, allowing only Warning, Error and Fatal levels in this case.
     * The other levels will be ignored.
     * All and none will be discarded.
    */
    // logger.setMinimumLogLevel(Warning);
    logger.log<Trace>("This is a trace message.");
    logger.log<Debug>("This is a debug message.");
    logger.log<Info>("This is an info message.");
    logger.log<Info>("This is an info message with additional fields.",
               {{"value1", std::to_string(1)}, {"value2", "example"}});
    logger.log<Warning>("This is a warning message.");
    logger.log<Error>("This is an error message.");
    logger.log<Fatal>("This is a fatal message.");
    return 0;
}
