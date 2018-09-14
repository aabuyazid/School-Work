#!/bin/bash

TA_MODE=false

PREPARING=false
CLEANING=false

declare -a SAMPLE_TEST_CASES=('Test_Base_Normal' \
'Test_Base_Punctuation');

function try() { [[ $- = *e* ]]; SAVED_OPT_E=$?;set +e;}
function throw() { exit $1;}
function catch() { export ex_code=$?;(( $SAVED_OPT_E )) && set +e;return $ex_code;}
function enable_throwing_errors() { set -e;}
function disable_throwing_errors() { set +e;}

main_dir=$(pwd)
submissions_dir="$main_dir/submissions"
tests_dir="$main_dir/test_cases"
brief_feedback_file="$main_dir/brief_results.csv"
detailed_feedback_dir="$main_dir/detailed_feedback"
test_timeout_seconds=3

collect_results()
{
	mkdir -p "$detailed_feedback_dir"
	cd "$submissions_dir"
	for tested_submission in */
	do
		tested_submission=${tested_submission:0:${#tested_submission}-1}
		zip -ry9Tm "$tested_submission" "$tested_submission" > /dev/null
		mv "$tested_submission".zip "$detailed_feedback_dir"

	done
	cd "$main_dir"
}

perform_test_on_this_submission()
{
    enable_throwing_errors

	this_dir=$(pwd)
    total_num=0
    correct_num=0
    submission_name="${PWD##*/}"

    cp "$main_dir/helper_files/Helpers.h" "$main_dir/helper_files/main.cpp" "$main_dir/helper_files/Makefile" .
    if [[ $TA_MODE == true ]]; then
        make > /dev/null 2>&1
    else
        make
    fi
    rm *.h *.cpp Makefile

    for t_dir in "$tests_dir"/*/
    do
        cp "$t_dir/article.txt" "$t_dir/dictionary.txt" "$t_dir/out.txt" .

        total_num=$(($total_num+1))
        timeout $test_timeout_seconds ./a.out |
            tr '[:upper:]' '[:lower:]' | sort -u > actual_output.txt || true
        cat out.txt | tr '[:upper:]' '[:lower:]' | sort -u > expected_output.txt
        printf "\n\n\n$t_dir\n" >> output_diff.txt

        if [[ $TA_MODE == true ]]; then
            diff -iuBbN expected_output.txt actual_output.txt >> output_diff.txt &&
                correct_num=$(($correct_num+1)) || true
        else
            diff -iuBbN expected_output.txt actual_output.txt >> output_diff.txt &&
                correct_num=$(($correct_num+1)) || echo "error on $t_dir"
        fi

    done
    rm -rf a.out article.txt dictionary.txt out.txt actual_output.txt expected_output.txt
    echo "\"$submission_name\", \"$correct_num\", \"$total_num\"" >> "$brief_feedback_file"
}

run_test_on_submissions()
{
    enable_throwing_errors

	cd "$submissions_dir"

    if ( ls *.cpp 1> /dev/null 2>&1 )
    then
	    for submission_raw_name in *.cpp
	    do
	    	submission_name="${submission_raw_name%%.*}"
            try
            (
	    	    submission_name="${submission_raw_name%%.*}"
                echo ""
	    	    echo "### going on: " "$submission_name"

	    	    mkdir "$submission_name"
                mv "$submission_raw_name" "$submission_name/Project1.cpp"
	    	    cd "$submission_name"

	    	    perform_test_on_this_submission

	    	    cd "$submissions_dir"
            )
            catch ||
            {
                echo "###### submission format / compilation error on $submission_name"
                echo "\"$submission_name\", \"0\", \"0\"" >> "$brief_feedback_file"
                cd "$submissions_dir"
            }
	    done
    fi

	cd "$main_dir"
}

grade()
{
    enable_throwing_errors

    if [[ ! -f "$brief_feedback_file" ]]
    then
	    rm -rf  "$detailed_feedback_dir"
        echo '"submission_name", "passed_tests", "total_tests"' > "$brief_feedback_file"
    fi

	run_test_on_submissions
	echo "collecting results ..."
	collect_results
	echo "finished"

    disable_throwing_errors
}

main()
{
    if [[ $CLEANING == true ]]; then
        clean
    elif [[ $PREPARING == true ]]; then
        prepare
    else
        grade
    fi
}

prepare()
{
    if [[ $TA_MODE == true ]]; then
        rm -rf test_cases
        rm -rf submissions/*.cpp
        git checkout test_cases
    else
        mkdir "sample_tests"
        for tname in "${SAMPLE_TEST_CASES[@]}"; do
            mv "test_cases/$tname" "sample_tests"
        done
        rm -rf "test_cases"
        mv "sample_tests" "test_cases"
    fi
}

clean()
{
    rm -rf detailed_feedback brief_results.csv
    rm -rf submissions test_cases
    git checkout -- submissions test_cases
}

while [[ $# > 0 ]]
do
    arg="$1"
    case $arg in
        --ta)       TA_MODE=true;;
        --clean)    CLEANING=true;;
        --prepare)  PREPARING=true;;
    esac

    shift
done

main
