#! /bin/sh

inputs_ok='./tests/test_that_return0/*.py'
inputs_lex='./tests/test_that_return1/*.py'
inputs_syn='./tests/test_that_return2/*.py'
inputs_sem='./tests/test_that_return3/*.py'
inputs_sem_comp='./tests/test_that_return4/*.py'
inputs_sem_func='./tests/test_that_return5/*.py'
inputs_sem_other='./tests/test_that_return6/*.py'
inputs_zero_dev='./tests/test_that_return9/*.py'
inputs_sem_internal='./tests/test_that_return99/*.py'

i=0 # counter

echo "                                                                      "
echo "----------------OK PROGRAM TESTS ----------------"
echo "                                                                      "
# testy na programy
for file in $inputs_ok
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
echo "----------------LEXICAL ERROR TESTS ----------------"
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
echo "                                                                      "
echo "----------------SEM ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_sem
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "3" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=3, RETURNED=$return_value\n" "$test_name"
done
echo "                                                                      "
echo "----------------SEM_COMP ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_sem_comp
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "4" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=4, RETURNED=$return_value\n" "$test_name"
done
echo "                                                                      "
echo "----------------SEM_FUNC ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_sem_func
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "5" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=5, RETURNED=$return_value\n" "$test_name"
done
echo "                                                                      "
echo "----------------SEM_OTHER ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_sem_other
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "6" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=6, RETURNED=$return_value\n" "$test_name"
done
echo "                                                                      "
echo "----------------ZERO_DEV ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_zero_dev
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "9" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=9, RETURNED=$return_value\n" "$test_name"
done
echo "                                                                      "
echo "----------------INTERNAL ERROR TESTS ----------------"
echo "                                                                      "

# testy na syntax
for file in $inputs_sem_internal
do
    test_name=$(basename "$file")

    cat $file | ./p 2>/dev/null 1>/dev/null
	return_value=$?
	i=$(($i + 1))

    # vypsani testu
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "99" ]; then
    printf "   OK   "
	else
	printf "ERROR   "
	fi
	printf "%-16s | EXPECTED=99, RETURNED=$return_value\n" "$test_name"
done
