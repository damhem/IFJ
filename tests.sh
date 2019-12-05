#! /bin/sh

#interpreter=./ic17int
#tmp_file_ifjcode17=./tests/tmp_file_ifjcode17
#tmp_file_interpreter=./tests/tmp_file_interpreter


#inputs_succ_error0=./tests/vstupy/succ_error0/*.input
#outputs_succ_error0_path=./tests/vstupy/succ_error0
inputs_normal='./tests/test0/*.py'
inputs_lex='./tests/test_that_return1/*.py'
inputs_syn='./tests/test2/*.py'

#inputs_semm_error3=./tests/vstupy/semm_error3/*.input
#inputs_semm_error4=./tests/vstupy/semm_error4/*.input
i=0 # counter

echo "                                                                      "
echo "----------------COMPLETE PROGRAM TESTS ----------------"
echo "                                                                      "
# testy na programy
for file in $inputs_normal
do
    test_name=$(basename "$file")

	#test_output=
    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

	printf "%-02d  " "$i"
	if [ "$return_value" -eq "0" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=0, RETURNED=$return_value\n" "$test_name"
done

echo "                                                                      "
echo "----------------LEXXICAL ERROR TESTS ----------------"
echo "                                                                      "
# testy na scanner
for file in $inputs_lex
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

	printf "%-02d  " "$i"
	if [ "$return_value" -eq "1" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=1, RETURNED=$return_value\n" "$test_name"
done

echo "                                                                      "
echo "----------------SYNTAX ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_syn
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "2" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=2, RETURNED=$return_value\n" "$test_name"
done