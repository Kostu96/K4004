%option noyywrap 

%{

%}

%%

[ \t]  {} // ignore whitespace
";".*  {} // ignore comments
\r?\n  {} // ignore new lines
"$"    { return 0; } // end of the program

"NOP" |
"JCN" |
"FIM" |
"SRC" |
"FIN" |
"JIN" |
"JUN" |
"JMS" |
"INC" |
"ISZ" |
"ADD" |
"SUB" |
"LD"  |
"XCH" |
"BBL" |
"LDM" |
"WRM" |
"WMP" |
"WRR" |
"WR0" |
"WR1" |
"WR2" |
"WR3" |
"SBM" |
"RDM" |
"RDR" |
"ADM" |
"RD0" |
"RD1" |
"RD2" |
"RD3" |
"CLB" |
"CLC" |
"IAC" |
"CMC" |
"CMA" |
"RAL" |
"RAR" |
"TCC" |
"DAC" |
"TCS" |
"STC" |
"DAA" |
"KBP" |
"DCL" { printf("MNEMONIC: %s\n", yytext); }
	  
"R0"  |
"R1"  |
"P1"  |
"P2"  |
"P3"  |
"P4"  { printf("REGISTER: %s\n", yytext); }

"$"[0-9A-Fa-f]+       { printf("HEX_NUM: %s\n", yytext); }
[0-9]+                { printf("DEC_NUM: %d\n", atoi(yytext)); }
"0"[0-7]+              { printf("OCT_NUM: %d\n", yytext); } 
"%"[01]+              { printf("BIN_NUM: %d\n", yytext); } 
[A-Za-z][A-Za-z0-9_]+ { printf("LABEL: %s\n", yytext); }

"," { printf("COMMA\n"); }
"*" { printf("ASTERISK\n"); }
"=" { printf("ASSIGN\n"); }

. { printf("UNKNOWN: %s\n", yytext); }

%%

int main(int argc, char** argv)
{
	if(argc > 1) {
		if(!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return (1);
		}
	}
	
	while(yylex() != 0);
	
	//printf("%8d%8d%8d\n", lines, words, chars);
	
	return 0;
}
