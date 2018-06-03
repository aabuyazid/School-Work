HOW TO RUN THE GRADING SCRIPT

These are instructions for running the grading script that we will use for grading your assignment. You must run this script on kamek.ece successfully, getting a perfect score for the 2 tests given to you.

As described in the project description, make your Project1.cpp.
Make a directory in kamek to run this script.  You may call it ~/EE312/Project1/GradingScript, for example.  Copy the sample_grading_script.zip to this folder, and unzip it using the unzip command.
Place Project1.cpp into the 'submissions' directory.
Then run ./grade.sh .
CAUTION: your submission file Project1.cpp will be deleted after grading, so keep a copy!
There are 2 testcases in the testcases directory.
There are 4 sample fake submissions, so that you can see behavior of the script in case of compile/runtime errors as well. After running the script, the brief_results.csv file should be like this (line ordering may change):

"submission_name", "passed_tests", "total_tests"
"compile_error", "0", "0"
"full_credit", "2", "2"
"link_error", "0", "0"
"runtime_error", "0", "2"
"some_credit", "1", "2"

When we compare your program's results against out.txt, we remove duplicates, sort the output, remove blank lines etc.  
You may see brief_result.csv to see the abbreviated results and you may see detailed_feedback to see the difference between expected output and actual output for each submission for all of the tests.
