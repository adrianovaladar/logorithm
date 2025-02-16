#include <logorithm/Logger.h>

using enum LOGLEVEL;

int main() {
    /* Uncomment the next line to filter logs, allowing only Warning, Error and Fatal levels in this case.
     * The other levels will be ignored.
     * All and none will be discarded.
    */
    // logger.setMinimumLogLevel(Warning);
    logger.log("This is a trace message.", Trace);
    logger.log("This is a debug message.", Debug);
    logger.log("This is an info message.", Info);
    logger.log("This is an info message with additional fields.", Info,
               {{"value1", std::to_string(1)}, {"value2", "example"}});
    logger.log("This is a warning message.", Warning);
    logger.log("This is an error message.", Error);
    logger.log("This is a fatal message.", Fatal);
    return 0;
}
