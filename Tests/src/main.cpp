
#include "FENTests.h"
#include "StaticEvaluationTests.h"
#include "TestsUtilities.h"

int main()
{
    FENTests::Run();
    StaticEvaluationTests::Run();

    TestsUtilities::log("---------------");
    TestsUtilities::log("All tests done.");
    return 0;
}