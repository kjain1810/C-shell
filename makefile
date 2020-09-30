all:
	gcc *.c -o shell -g -w
	touch .shell_history.txt
	./shell

clean:
	rm .shell_history.txt
	rm -rf test_files
	rm shell
