HOW TO RUN THE GRADING SCRIPT

These are instructions for running the grading script that we will use for grading your assignment. You must run this script on kamek.ece successfully, getting a perfect score for the 4 tests given to you.

As described in the project description, make your submission zip file (containing at least one file with the main method; feel free to add more .cpp and .h files if you want.). BE CAREFULL, WE WILL REPLACE YOUR YOUR MAKEFILE. DO NOT RELY ON THE CHANGES YOU MAKE ON MAKEFILE. YOU SHOULD HAVE EXACTLY ONE MAIN METHOD IN YOUR FILES AND IT SHOULD MATCH THE ASSIGNMENT DESCRIPTION. (look at the "link_error" and "wrong_structure" sample submissions (as examples of bad submissions) for more info)
Make a directory in kamek to run this script.  You may call it ~/EE312/Project8/GradingScript, for example.  Copy the sample_grading_script.zip to this folder, and unzip it using the unzip command.
Place zip file into the 'submissions' directory.
Then run ./grade.sh .
CAUTION: your submission zip file will be deleted after grading, so keep a copy!
There are 4 tests in the testcases directory.
There are 6 sample fake submissions, so that you can see behavior of the script in case of compile/runtime errors as well. After running the script, the brief_results.csv file should be like this (line ordering may change):

"submission_name", "passed_tests", "total_tests"
"compile_error", "0", "0"
"infinite_loop", "0", "4"
"link_error", "0", "0"
"runtime_error", "0", "4"
"some_credit", "2", "4"
"wrong_structure", "0", "4"

You may see brief_result.csv to see the abbreviated results and you may see detailed_feedback to see the detailed description about each test failure.
