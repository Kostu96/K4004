# K4004 Assembler
### syntax grammar
```
program = line* EOF

line = '*' '=' NUMBER '\n'
     | LABEL ('=' NUMBER)? '\n'
     | LABEL instruction '\n'
     | pragma '\n'
     | instruction '\n'

pragma = ".BYTE" NUMBER

instruction = SIMPLE_MNEMONIC
            | COMPLEX_MNEMONIC argument
            | TWO_BYTE_MNEMONIC argument (, argument)?

argument = LABEL
         | NUMBER
         | REGISTER
         | REGISTER_PAIR
```
### lexical grammar
```
LETTER = ['A'-'Z']

DIGIT = ['0'-'9']

HEX_DIGIT = ['0'-'9','A'-'F']

LABEL = LETTER(LETTER | DIGIT | '_'])*

NUMBER = DIGIT+
       | '$'HEX_DIGIT+
       | '%'('0' | '1')+

REGISTER = 'R'HEX_DIGIT

REGISTER_PAIR = 'P'['0'-'7']

SIMPLE_MNEMONIC = "NOP" | "HLT" | "BBS" | "LCR" | "OR4" | "OR5" | "AN6" | "AN7"
                | "DB0" | "DB1" | "SB0" | "SB1" | "EIN" | "DIN" | "RPM" | "WRM"
                | "WMP" | "WRR" | "WPM" | "WR0" | "WR1" | "WR2" | "WR3" | "SBM"
                | "RDM" | "RDR" | "ADM" | "RD0" | "RD1" | "RD2" | "RD3" | "CLB"
                | "CLC" | "IAC" | "CMC" | "CMA" | "RAL" | "RAR" | "TCC" | "DAC"
                | "TCS" | "STC" | "DAA" | "KBP" | "DCL"

COMPLEX_MNEMONIC = "SRC" | "FIN" | "JIN" | "INC" | "ADD"
                 | "SUB" | "LDM" | "XCH" | "BBL" | "LD"

TWO_BYTE_MNEMONIC = "JCN" | "FIM" | "JUN" | "JMS" | "ISZ"
```
