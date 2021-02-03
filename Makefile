repl-build.out: Completion_Designer.c GravelPackSlotSize.c
	 gcc -o $@ $^

.PHONY: run
run: repl-build.out input.txt output.txt
	 ./$^

.PHONY: clean

clean:
	rm -f repl-build.out