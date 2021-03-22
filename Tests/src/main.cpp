
#include "FENTests.h"
#include "StaticEvaluationTests.h"
#include "TestsUtilities.h"

void printReleaseOrDebug()
{
#ifdef NDEBUG
    TestsUtilities::log("Tests are running in a 'Release' build.");
#else
    TestsUtilities::log("Tests are running in a 'Debug' build.");
#endif

}

int main()
{
    printReleaseOrDebug();

    StaticEvaluationTests::Run();
    FENTests::Run();

    TestsUtilities::log("---------------");
    TestsUtilities::log("All tests done.");
    return 0;
}