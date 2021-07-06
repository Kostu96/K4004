; MCS-4 exerciser program

       WRR
       BBL 15
       FIM P5, $41
       JMS LD_MK
       JMS CK_IDX
       FIN P0
.BYTE  $FE
       JMS CK_FIN
       JMS CK_IDX
       JMS CK_FIN
       JMS CK_IDX
       FIM P5, $42
       JMS $FFF
       JMS $1A
       JUN $824
       JMS $FFF
       JMS $320
       JUN $C18
       JMS $FFF
       JUN $FFF
       FIM P1, $CB
       CLB
       SRC P5
       WMP
       SRC P0
       WRM
       IAC
       ISZ R1, $29
       WR0
       IAC
       WR1
       IAC
       WR2
       IAC
       WR3
       INC R0
       ISZ R2, $29
       STC
       JMS CK_DCL
       ISZ R3, $39
       SRC P2
       STC
       RAL
       WMP
       JCN %1010, $47 ; Jump if CY == 0
       JCN %1100, $4F ; Jump if A != 0
       JCN %1001, $50 ; Jump if T == 1
       JCN %0010, $50 ; Jump if CY == 1
       JCN %0100, $52 ; Jump if A == 0
       JCN %0001, $43 ; Jump if T == 0
       JUN $045
       CLB
       JUN $03F
       FIM P6,$66
       FIM P7,$59
       FIM P0,$00
       JIN P6
       SRC P0
       SUB R4
       SUB R5
       WRM
       CLB
       ISZ R4, $68
       JMS LD_MK

*=222
LD_MK  SRC P5
       LD R11
       CLC
       WMP
       RAL
       XCH R11
       BBL 0

CK_IDX SRC P0
       SRC P1
       SRC P2
       SRC P3
       SRC P4
       SRC P5
       SRC P6
       SRC P7
       BBL 0

CK_FIN FIN P1
       FIN P2
       FIN P3
       FIN P4
       FIN P5
       FIN P6
       FIN P7
       FIN P0
       BBL 0

CK_DCL LD R4
       RAL
       DCL
       XCH R4
       RDR
       BBL 0

       NOP
.BYTE  $FE
       NOP
