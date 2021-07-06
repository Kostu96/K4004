; MCS-4 exerciser program

       WRR            ; 000
       BBL 15         ; 001
       FIM P5, $41    ; 002
       JMS LD_MK      ; 004
       JMS CK_IDX     ; 006
       FIN P0         ; 008
.BYTE  $FE            ; 009
       JMS CK_FIN     ; 010
       JMS CK_IDX     ; 012
       JMS CK_FIN     ; 014
       JMS CK_IDX     ; 016
       FIM P5, $42    ; 018
       JMS $FFF       ; 020
       JMS $71A       ; 022
       JUN $824       ; 024
       JMS $FFF       ; 026
       JMS $320       ; 028
       JUN $C18       ; 030
       JMS $FFF       ; 032
       JUN $FFF       ; 034
       FIM P1, $CB    ; 036
       CLB            ; 038
       SRC P5         ; 039
       WMP            ; 040
       SRC P0         ; 041
       WRM            ; 042
       IAC            ; 043
       ISZ R1, $29    ; 044
       WR0            ; 046
       IAC            ; 047
       WR1            ; 048
       IAC            ; 049
       WR2            ; 050
       IAC            ; 051
       WR3            ; 052
       INC R0         ; 053
       ISZ R2, $29    ; 054
       STC            ; 056
       JMS CK_DCL     ; 057
       ISZ R3, $39    ; 059
       SRC P2         ; 061
       STC            ; 062
       RAL            ; 063
       WMP            ; 064
       JCN %1010, $47 ; 065 ; Jump if CY == 0
       JCN %1100, $4F ; 067 ; Jump if A != 0
       JCN %1001, $50 ; 069 ; Jump if T == 1
       JCN %0010, $50 ; 071 ; Jump if CY == 1
       JCN %0100, $52 ; 073 ; Jump if A == 0
       JCN %0001, $43 ; 075 ; Jump if T == 0
       JUN $045       ; 077
       CLB            ; 079
       JUN $03F       ; 080
       FIM P6, $66    ; 082
       FIM P7, $59    ; 084
       FIM P0, $00    ; 086
       JIN P6         ; 088
       SRC P0         ; 089
       ADD R4         ; 090
       ADD R5         ; 091
       WRM            ; 092
       RAR            ; 093
       ISZ R4, 89     ; 094
       ISZ R5, 89     ; 096
       JMS LD_MK      ; 098
                      ; 100

*=222
LD_MK  SRC P5         ; 222
       LD R11         ; 223
       CLC            ; 224
       WMP            ; 225
       RAL            ; 226
       XCH R11        ; 227
       BBL 0          ; 228

CK_IDX SRC P0         ; 229
       SRC P1         ; 230
       SRC P2         ; 231
       SRC P3         ; 232
       SRC P4         ; 233
       SRC P5         ; 234
       SRC P6         ; 235
       SRC P7         ; 236
       BBL 0          ; 237

CK_FIN FIN P1         ; 238
       FIN P2         ; 239
       FIN P3         ; 240
       FIN P4         ; 241
       FIN P5         ; 242
       FIN P6         ; 243
       FIN P7         ; 244
       FIN P0         ; 245
       BBL 0          ; 246

CK_DCL LD R4          ; 247
       RAL            ; 248
       DCL            ; 249
       XCH R4         ; 250
       RDR            ; 251
       BBL 0          ; 252

       NOP            ; 253
.BYTE  $FE            ; 254
       NOP            ; 255
