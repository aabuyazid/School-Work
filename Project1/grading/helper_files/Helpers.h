/*
 *
 */
#pragma once



///// COPIED FROM Grader.h /////
/* Represents the result of one single test ordered by severity */
enum TestResult
{
    TEST_PASSED = 0,        /* Test passed */
    TEST_FAILED = 1,        /* Test failed - general failure */
    TEST_MEMLEAK = 2,       /* Test failed - memory leak */
    TEST_OUTPUT = 3,        /* Test failed - incorrect output */
    TEST_TIMEOUT = 4,       /* Test failed - timed out */
    TEST_CRASHED = 5,       /* Test failed - test crashed */
    TEST_COMPILE = 6,       /* Test failed - compilation failed */

    TEST_MISSING = 7,       /* Test failed - no result for this student */
};


