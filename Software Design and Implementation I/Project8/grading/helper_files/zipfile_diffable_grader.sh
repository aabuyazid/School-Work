run_test_on_submissions()
{
    enable_throwing_errors

	cd "$submissions_dir"

    if ( ls *.zip 1> /dev/null 2>&1 )
    then
	    for submission_raw_name in *.zip
	    do
	    	submission_name="${submission_raw_name%%.*}"
            try
            (
	    	    submission_name="${submission_raw_name%%.*}"
                echo ""
	    	    echo "### going on: " "$submission_name"

	    	    mkdir "$submission_name"
	    	    cd "$submission_name"
                unzip_student_file "../$submission_raw_name"
                flatten_all_file_types wanted_file_types[@]
                rm "../$submission_raw_name"

                for overriding in $overriding_files; do
                    rm -f "$overriding"
                done

                for helper in $(ls "$main_dir/helper_files/compile_helpers"); do
                    if [[ ( ! -f "$helper") ]]; then
                        cp "$main_dir/helper_files/compile_helpers/$helper" .
                    fi
                done

	    	    perform_test_on_this_submission

	    	    cd "$submissions_dir"
            )
            catch ||
            {
                echo "###### submission format / compilation error on $submission_name"
                echo "$(brief_result_error_row \"$submission_name\")" >> "$brief_feedback_file"
                cd "$submissions_dir"
            }
	    done
    fi

	cd "$main_dir"
}

perform_test_on_this_submission()
{
    enable_throwing_errors

	this_dir=$(pwd)
    total_num=0
    correct_num=0
    submission_name="${PWD##*/}"

    if [[ $TA_MODE == true ]]; then
        make > /dev/null 2>&1
    else
        make 
    fi
    rm -f .*.d *.o *.h *.cpp Makefile

    for t_dir in "$tests_dir"/*/
    do
        cp "$t_dir/"* .

        total_num=$(($total_num+1))
        printf "\n\n\n$t_dir\n" >> output_diff.txt

        if [[ $TA_MODE == true ]]; then
            timeout $test_timeout_seconds ./a.out input.txt \
                > actual_output.txt 2> /dev/null || true
            python2 "$main_dir/helper_files/rdiff.py" out.txt actual_output.txt >> output_diff.txt &&
                correct_num=$(($correct_num+1)) || true
        else
            timeout $test_timeout_seconds ./a.out input.txt \
                > actual_output.txt || true
            python2 "$main_dir/helper_files/rdiff.py" out.txt actual_output.txt >> output_diff.txt &&
                correct_num=$(($correct_num+1)) || echo "error on $t_dir"
        fi

    done
    rm -rf a.out* input.txt actual_output.txt out.txt *.blip

    echo "\"$submission_name\", \"$correct_num\", \"$total_num\"" >> "$brief_feedback_file"
}

save_function clean before_previous_clean
clean()
{
    before_previous_clean
    rm -rf "$main_dir/helper_files/rdiff.py"
}

save_function prepare previous_prepare
prepare()
{
    cp "$LIB_DIR/rdiff.py" "$main_dir/helper_files" > /dev/null 2>&1 || true
    if [[ $TA_MODE == true ]]; then
        rm -rf submissions/*
    else
        cd submissions
        for sub in */
        do
            zip -ry9Tm $(basename $sub) $(basename $sub)
        done
        cd ..
    fi
    previous_prepare
}
