; MCS-4 exerciser program

       WRR            ; 000
       BBL 15         ; 001
       FIM P5, $41    ; 002
       JMS LD_MK      ; 004
       JMS CK_IDX     ; 006
       FIM P0, $FE    ; 008 ; There is a typo here in the original listing - should be FIM, but FIN is printed.
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
       JUN $075       ; 100
	   JMS LD_MK      ; 102
	   SRC P0         ; 104
	   SUB R4         ; 105
	   SUB R5         ; 106
	   WRM            ; 107
	   CLB            ; 108
	   ISZ R4, 104    ; 109
	   ISZ R5, 104    ; 111
	   CLB            ; 113
	   SRC P5         ; 114
	   WMP            ; 115
	   JIN P7         ; 116
	   STC            ; 117
	   INC R8         ; 118
	   LD  R8         ; 119
	   WRM            ; 120
	   XCH R9         ; 121
	   LD  R9         ; 122
	   WRR            ; 123
	   DAA            ; 124
	   WRM            ; 125
	   ISZ R4, 117    ; 126
	   CLB            ; 128
	   DAC            ; 129
	   WRM            ; 130
	   KBP            ; 131
	   WRM            ; 132
	   ISZ R4, 129    ; 133
	   CLB            ; 135
	   DAA            ; 136
	   WRM            ; 137
	   IAC            ; 138
	   ISZ R4, 136    ; 139
	   LDM 15         ; 141
	   WRM            ; 142
	   TCC            ; 143
	   WRM            ; 144
	   JCN %1100, 141 ; 145 ; Jump if A != 0
	   CLB            ; 147
	   SRC P5         ; 148
	   WMP            ; 149
	   LDM 15         ; 150
	   TCS            ; 151
	   WRM            ; 152
	   STC            ; 153
	   TCS            ; 154
	   WRM            ; 155
	   CMC            ; 156
	   RAR            ; 157
	   WRM            ; 158
	   ISZ R4, 156    ; 159
	   FIM P2, $C0    ; 161
	   SRC P0         ; 163
	   RDM            ; 164
	   ISZ R1, 163    ; 165
	   RD0            ; 167
	   RD1            ; 168
	   RD2            ; 169
	   RD3            ; 170
	   INC R0         ; 171
	   ISZ R4, 163    ; 172
	   FIM P0, $20    ; 174
	   FIM P1, $30    ; 176
	   SRC P0         ; 178
	   SBM            ; 179
	   INC R1         ; 180
	   SRC P1         ; 181
	   SBM            ; 182
	   WRM            ; 183
	   ISZ R3, 178    ; 184
	   FIM P0, 0      ; 186
	   FIM P0, $10    ; 188
	   CLB            ; 190
	   SRC P5         ; 191
	   WMP            ; 192
	   SRC P0         ; 193
	   ADM            ; 194
	   INC R1         ; 195
	   SRC P1         ; 196
	   ADM            ; 197
	   WRM            ; 198
	   ISZ R3, 193    ; 199
	   SRC P5         ; 201
	   RD0            ; 202
	   JCN %0100, 215 ; 203 ; Jump if A == 0
	   LDM 8          ; 205
	   SRC P0         ; 206
	   WMP            ; 207
	   CLB            ; 208
	   SRC P5         ; 209
	   WR0            ; 210
	   JCN %1001, 211 ; 211 ; Jump if T == 1
	   JUN 2          ; 213
	   IAC            ; 215
	   WR0            ; 216
	   LDM 2          ; 217
	   SRC P0         ; 218
	   WMP            ; 219
	   JUN 2          ; 220

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
.BYTE  $FF            ; 254
       NOP            ; 255
