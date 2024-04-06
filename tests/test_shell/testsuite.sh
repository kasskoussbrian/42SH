#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"
BINAIRE="../../../src/42sh"

TOTAL_RUN=0
TOTAL_FAIL=0

ref_file_out_filename=/tmp/.ref_file_out_filename
ref_file_err_filename=/tmp/.ref_file_err_filename

my_file_out_filename=/tmp/.my_file_out_filename
my_file_err_filename=/tmp/.my_file_err_filename

ref_file_out_string=/tmp/.ref_file_out_string
ref_file_err_string=/tmp/.ref_file_err_string

my_file_out_string=/tmp/.my_file_out_string
my_file_err_string=/tmp/.my_file_err_string

ref_file_out_stdin=/tmp/.ref_file_out_stdin
ref_file_err_stdin=/tmp/.ref_file_err_stdin

my_file_out_stdin=/tmp/.my_file_out_stdin
my_file_err_stdin=/tmp/.my_file_err_stdin

run_test() {
    run_test_filename "$1"
    run_test_stdin "$1"
    run_test_string "$1"
}

run_test_filename() {
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    TOTAL_RUN=$((TOTAL_RUN+1))

    echo -e "$BLUE-->> ${WHITE}$1...$WHITE"
    echo "src/42sh $(realpath $1)"
    # cat $1
    # echo ""

    bash --posix "$1" > $ref_file_out_filename 2> $ref_file_err_filename
    REF_CODE=$?
    ${BINAIRE} "$1" > $my_file_out_filename 2> $my_file_err_filename
    MY_CODE=$?

    diff --color=always -u $ref_file_out_filename $my_file_out_filename > $1.diff
    DIFF_CODE=$?

    grep Sanitizer /tmp/.my_file_err_filename 2>&1 >/dev/null
    LEAK_CODE=$?

    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success=false
    fi
    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success=false
    fi
    if { [ -s $ref_file_err_filename ] && [ ! -s $my_file_err_filename ]; } ||
        { [ ! -s $ref_file_err_filename ] && [ -s $my_file_err_filename ]; }; then
        echo -ne "$RED STDERR$WHITE"
        success=false
    fi
    if [ $LEAK_CODE == 0 ]; then
        echo -ne "$RED LEAK$WHITE"
        success=false
    fi

    if $success; then
        # echo -e "$GREEN $command OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED $(cat $(realpath $1.diff))$WHITE"
        echo
        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi
}

run_test_string() {
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    TOTAL_RUN=$((TOTAL_RUN+1))

    string=$(cat $1)
    echo -e "$BLUE -->> ${WHITE}$1"
    echo "src/42sh -c \"${string}\""

    bash --posix -c "${string}" > $ref_file_out_string 2> $ref_file_err_string
    REF_CODE=$?
    ${BINAIRE} -c "${string}" > $my_file_out_string 2> $my_file_err_string
    MY_CODE=$?

    diff --color=always -u $ref_file_out_string $my_file_out_string > $1.diff
    DIFF_CODE=$?

    grep Sanitizer /tmp/.my_file_err_filename 2>&1 >/dev/null
    LEAK_CODE=$?

    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success=false
    fi
    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success=false
    fi
    if { [ -s $ref_file_err_string ] && [ ! -s $my_file_err_string ]; } ||
        { [ ! -s $ref_file_err_string ] && [ -s $my_file_err_string ]; }; then
        echo -ne "$RED STDERR$WHITE"
        success=false
    fi
    if [ $LEAK_CODE == 0 ]; then
        echo -ne "$RED LEAK$WHITE"
        success=false
    fi

    if $success; then
        # echo -e "$GREEN $command OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED $(cat $(realpath $1.diff))$WHITE"
        echo
        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi
}

run_test_stdin() {
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    TOTAL_RUN=$((TOTAL_RUN+1))


    echo -e "$BLUE -->> ${WHITE}$1"
    echo "cat $(realpath $1) | src/42sh"

    cat "$1" | bash --posix > $ref_file_out_stdin 2> $ref_file_err_stdin
    REF_CODE=$?
    cat "$1" | ${BINAIRE} > $my_file_out_stdin 2> $my_file_err_stdin
    MY_CODE=$?

    diff --color=always -u $ref_file_out_stdin $my_file_out_stdin > $1.diff
    DIFF_CODE=$?

    grep Sanitizer /tmp/.my_file_err_filename 2>&1 >/dev/null
    LEAK_CODE=$?

    if [ $LEAK_CODE == 0 ]; then
        echo -ne "$RED LEAK$WHITE"
        success=false
    fi

    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success=false
    fi
    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success=false
    fi
    if { [ -s $ref_file_err_stdin ] && [ ! -s $my_file_err_stdin ]; } ||
        { [ ! -s $ref_file_err_stdin ] && [ -s $my_file_err_stdin ]; }; then
        echo -ne "$RED STDERR$WHITE"
        success=false
    fi
    if [ $LEAK_CODE == 0 ]; then
        echo -ne "$RED LEAK$WHITE"
        success=false
    fi

    if $success; then
        # echo -e "$GREEN $command OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED $(cat $(realpath $1.diff))$WHITE"
        echo
        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi
}

run_category() {
    cd $1
    source ./testsuite.sh
    cd - >/dev/null
}

run_testsuite() {
    for category in $@; do
        [ ${category} = "." ] && continue

        echo -e "$TURQUOISE======================================="
        printf " $WHITE%-36s $TURQUOISE%s\n" "${category#*/}"
        echo -e "$TURQUOISE======================================="

        run_category $category
    done
}

run_testsuite $(find . -type d)

echo -e "$WHITE"

PERCENT_SUCCES=$(((TOTAL_RUN - TOTAL_FAIL) * 100 / TOTAL_RUN))

echo -e "$BLUE============================================"
echo -e "$WHITE RECAP: $([ $PERCENT_SUCCES = 100 ] && echo $GREEN || echo $RED) $PERCENT_SUCCES%"
echo -e "$BLUE============================================"

rm -f $ref_file_out $my_file_out $ref_file_err $ref_file_out

echo -e "$WHITE"
