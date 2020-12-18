%{
#include <stdio.h> 
%}

%token ADD FIM LD XCH JUN

%%

program:
;

%%

int main(int argc, char **argv)
{
	yyparse();
	return 0;
}

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}
