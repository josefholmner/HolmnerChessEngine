
#include "TestsUtilities.h"
#include "StaticEvaluationTests.h"
#include "GetBestMovePerformanceTests.h"
#include "RawMinimaxPerformanceTests.h"
#include "FENTests.h"

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
    GetBestMovePerformanceTests::Run();
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