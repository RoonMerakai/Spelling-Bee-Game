build:
	rm -f spellingBee.exe
	gcc main.c -o spellingBee.exe

run:
	./spellingBee.exe

run_simp:
	./spellingBee.exe -d newsamp.txt

valgrind:
	rm -f spellB_debug.exe
	gcc -g main.c -o spellB_debug.exe
	echo "watched w" > sampleIn.txt
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./spellB_debug.exe < sampleIn.txt

clean:
	rm -f spellingBee.exe
	rm -f spellB_debug.exe

# TODO: Task 0 - extend the makefile for the following targets:
#         - run_play to execute the program spellingBee.exe with 
#           play mode ON, random mode ON using a hive size of 7, 
#           and default values for all other command-line arguments. 
#         - run_sample to execute the program spellingBee.exe with a small
#           dictionary file newsamp.txt and default values for all other
#           command-line arguments, but redirects input from a file that is
#           created to store the input hive "acdeit" with required letter 'e' 
#           (similar to the valgrind target).
#         - any additional targets you find useful as you write code.

run_play:
	./spellingBee.exe -r 7 -p

run_sample:
	echo "acdeit e" > sampleIn.txt
	./spellingBee.exe -d newsamp.txt < sampleIn.txt

gprof_build:
	gcc -pg main.c -o spBee_gprof.exe

gprof_brute:
	echo "ecmoprt e" > gprof_hive.txt
	./spBee_gprof.exe < gprof_hive.txt
	gprof ./spBee_gprof.exe > gprof_bruteforce.txt

gprof_optimized:
	echo "ecmoprt e" > gprof_hive.txt
	./spBee_gprof.exe -o < gprof_hive.txt
	gprof ./spBee_gprof.exe > gprof_optimized.txt


