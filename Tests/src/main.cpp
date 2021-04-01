
#include "TestsUtilities.h"
#include "StaticEvaluationTests.h"
#include "GetBestMovePerformanceTests.h"
#include "RawMinimaxPerformanceTests.h"
#include "FENTests.h"
#include "AlphaBetaMiniMaxEquivalencyTests.h"

void printReleaseOrDebugBuild()
{
    if (TestsUtilities::isReleaseBuild())
    {
        TestsUtilities::log("Tests are running in a 'Release' build.\n");
    }
    else
    {
        TestsUtilities::log("Tests are running in a 'Debug' build.\n");
    }
}

void runAllTests()
{
    StaticEvaluationTests::Run();
    GetBestMovePerformanceTests::Run();
    RawMinimaxPerformanceTests::Run();
    FENTests::Run();
    AlphaBetaMiniMaxEquivalencyTests::Run();

    TestsUtilities::log("---------------");
    TestsUtilities::log("All tests done.");
}

int main()
{
    printReleaseOrDebugBuild();
    runAllTests();
    return 0;
}