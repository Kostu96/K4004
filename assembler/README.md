# K4004 Assembler
### syntax grammar
```
program = line* EOF

line = LABEL [EQUAL NUMBER] '\n'
     | LABEL instruction '\n'
     | pragma '\n'
     | instruction '\n'

pragma = ".BYTE" NUMBER

instruction = MNEMONIC [argumentList]

argumentList = argument [, arguement]

argument = LABEL
         | NUMBER
         | REGISTER
         | REGISTER_PAIR
```
### lexical grammar
