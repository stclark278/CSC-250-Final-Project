repl-build.out: Completion_Designer.c SeiveAnalysis.c
	 gcc -o $@ $^

.PHONY: run
run: repl-build.out input.txt output.txt
	 ./$^

.PHONY: clean

clean:
	rm -f repl-build.out