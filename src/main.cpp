#include <stdio.h>

#include <pantor/inja.hpp>

int _read(char* p, size_t n) {
	return	fread((void*)p, 1, n, stdin);
}
int _write(char* p, size_t n) {
	return	fwrite((void*)p, 1, n, stdout);
}

FILE*	inn	= stdin;
FILE*	out	= stdout;

int main(int argc, char*argv[]) {

	printf("%d\n", argc);


	fclose(inn);
	fclose(out);
	return	0;
}
