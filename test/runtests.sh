echo "RUNNING UNIT TESTS"
echo ""

for i in test/*_test
do 
	if test -f $i
	then 
		if $VALGRIND ./$i 2>> test/test.log
		then 
			# echo $i PASS
			echo ""
		else
			echo "ERROR in test $i: here's tests/qtests.log"
			echo "----"
			tail test/test.log
			exit 1
		fi
	fi
done
echo ""




