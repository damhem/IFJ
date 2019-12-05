#! /bin/sh

proj=./proj
interpreter=./ic17int
tmp_file_ifjcode17=./tests/tmp_file_ifjcode17
tmp_file_interpreter=./tests/tmp_file_interpreter


inputs_succ_error0=./tests/vstupy/succ_error0/*.input
outputs_succ_error0_path=./tests/vstupy/succ_error0
inputs_lexx_error1=./tests/vstupy/lexx_error1/*.input
inputs_synt_error2=./tests/vstupy/synt_error2/*.input
inputs_semm_error3=./tests/vstupy/semm_error3/*.input
inputs_semm_error4=./tests/vstupy/semm_error4/*.input
i=0 # counter

echo "----------SUCCESS ERROR TESTS 0----------"

# testy na lexikalni chyby
for f in $inputs_succ_error0
do
	# PROMENNE
	filename=$(basename "$f")
	test_name="${filename%.*}"
	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number
    
	# POKUD BYL VRACEN CODE 0: VYHODNOCENI INTSTRUKCI INTERPRETEREM
	if [ $return_value -eq 0 ]; then
		printf "%s" "$test_output" > "$tmp_file_ifjcode17" # vypsani ifjcode17 do tmp souboru
		./$interpreter "$tmp_file_ifjcode17" > "$tmp_file_interpreter" 2>&1 # vypsani interpreter output do tmp souboru
		interpreter_result=$(diff $tmp_file_interpreter $outputs_succ_error0_path/$test_name.output)
	fi

    # VYPSANI TESTU
	printf "%-02d  " "$i" # vypis cisla testu
	if [ "$return_value" -eq "0" ] && [ -z "$interpreter_result" ]; then # vypis OK/ERROR
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi

	printf "%s.input | EXPECTED=0, RETURNED=$return_value" "$test_name" # vypis ERROR CODE
	
	if [ $return_value -eq 0 ]; then # vypis INTERPRETU
		if [ -z "$interpreter_result" ]; then
			printf " INTERPRETER OUTPUT OK"
		else
			printf " INTERPRETER OUTPUT DIFFER\n\n"
			echo "#START INTERPRETER OUTPUT"
			cat "$tmp_file_interpreter"
			printf "\n"
			echo "##END INTERPRETER OUTPUT"
			printf "\n"
			echo "#START EXPECTED OUTPUT"
			cat "$outputs_succ_error0_path/$test_name.output"
			printf "\n"
			echo "##END EXPECTED OUTPUT"
		fi
	fi
	printf "\n"
done

echo "----------LEXXICAL ERROR TESTS 1----------"
# testy na lexikalni chyby
for f in $inputs_lexx_error1
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "1" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=1, RETURNED=$return_value\n" "$test_name"
done

echo "----------SYNTAX ERROR TESTS 2----------"
# testy na syntakticke chybu
for f in $inputs_synt_error2
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "2" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=2, RETURNED=$return_value\n" "$test_name"

done

echo "----------SEMANTIC ERROR TESTS 3----------"
# testy na syntakticke chybu
for f in $inputs_semm_error3
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "3" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=3, RETURNED=$return_value\n" "$test_name"

done

echo "----------SEMANTIC ERROR TESTS 4----------"
# testy na syntakticke chybu
for f in $inputs_semm_error4
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "4" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=4, RETURNED=$return_value\n" "$test_name"

done