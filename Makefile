generate: 
	gcc -o moushell src/main.c src/builtins.c && mv moushell build
run: 
	./build/moushell
gr: 
	rm -f build/moushell && gcc -o moushell src/main.c src/builtins.c && mv moushell build && ./build/moushell

clear:
	rm -f build/moushell