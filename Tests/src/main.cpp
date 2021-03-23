
#include "StaticEvaluationTests.h"
#include "RawMinimaxPerformanceTests.h"
#include "FENTests.h"
#include "TestsUtilities.h"

void printReleaseOrDebugBuild()
{
#ifdef NDEBUG
    TestsUtilities::log("Tests are running in a 'Release' build.\n");
#else
    TestsUtilities::log("Tests are running in a 'Debug' build.\n");
#endif

}

void runAllTests()
{
    StaticEvaluationTests::Run();
    RawMinimaxPerformanceTests::Run();
    FENTests::Run();

    TestsUtilities::log("---------------");
    TestsUtilities::log("All tests done.");
}

int main()
{
    printReleaseOrDebugBuild();
    runAllTests();
    return 0;
}