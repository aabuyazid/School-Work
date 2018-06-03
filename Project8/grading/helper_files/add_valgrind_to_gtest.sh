get_source_backup=true
save_function perform_test_on_this_submission previous_perform_test_on_this_submission
save_function brief_result_header_row previous_brief_result_header_row
save_function brief_result_error_row previous_brief_result_error_row

brief_result_header_row() {
    echo '"submission_name", "passed_tests", "total_tests", "uninitialized_count", "invalid_read_count", "invalid_write_count", "invalid_free_count", "bytes_leaked"'
}

brief_result_error_row() {
    echo "$1, \"0\", \"0\", \"N/A\", \"N/A\", \"N/A\", \"N/A\", \"N/A\""
}

save_function prepare previous_prepare
prepare() {
    cp "$LIB_DIR/valgrind_processor.py" "$main_dir/helper_files" > /dev/null 2>&1 || true
    previous_prepare
}

save_function clean before_previous_clean
clean()
{
    before_previous_clean
    rm -rf "$main_dir/helper_files/valgrind_processor.py"
}

perform_test_on_this_submission() {
    previous_perform_test_on_this_submission "no output" 

    if [[ $total_num == 0 ]]; then
        brief_result_error_row "$submission_name" >> "$brief_feedback_file"
    else
        cp -R "$main_dir/helper_files/"* .
        mv "$CPP_FILE_NAME"_bak "$CPP_FILE_NAME"
        make sample_main &> /dev/null || true
        wipe_after_compile
        rm -rf gtest*

        echo "timeout $test_timeout_seconds valgrind ./sample_main.out" | bash &> valgrind_result.txt || true

        read -r uni i_r i_w i_f b_l <<<$(python "$main_dir/helper_files"/valgrind_processor.py ./valgrind_result.txt)

            rm -rf *.out*

            echo "\"$submission_name\", \"$correct_num\", \"$total_num\", \"$uni\", \"$i_r\", \"$i_w\", \"$i_f\", \"$b_l\"" \
                >> "$brief_feedback_file"
        fi

}
