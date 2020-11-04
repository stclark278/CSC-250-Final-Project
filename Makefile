repl-build.out: csc-150-review_template.c
	# gcc -o $@ $^
	echo "modify repl-build.out in the Makefile to make this button work"

.PHONY: run
run: repl-build.out
	# ./$^
	echo "modify run in the Makefile to make this button work"

.PHONY: clean

clean:
	rm -f repl-build.out