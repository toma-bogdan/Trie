build:
	gcc trie.c tema3.c -o tema3 -Wall
run:
	./tema3 input_file output_file
clean:
	rm -rf tema3