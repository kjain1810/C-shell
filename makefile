all:
	gcc *.c -o shell -g -w
	touch .shell_history.txt

clean:
	rm .shell_history.txt
	rm -rf test_files
	rm shell
