#!/bin/bash

LIB_DIR="helper_files"
. "$LIB_DIR"/library.sh
. "$LIB_DIR"/zipfile_diffable_grader.sh

declare -a SAMPLE_TEST_CASES=('Test_Provided_1' 'Test_Provided_2' 'Test_Provided_3' 'Test_Provided_4');
declare -a wanted_file_types=("*.cpp" "*.h" "Makefile")
overriding_files="Makefile"

test_timeout_seconds=3

main "$@"
