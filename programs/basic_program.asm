; four bit "AND" routine on the Intel 4004
;
START FIM P4, 0  ; LOAD ROM PORT 0 ADDRESS
      SRC P4     ; SEND ROM PORT ADDRESS
      RDR        ; READ INPUT A
      XCH R0     ; A TO REGISTER 0
      INC 8      ; LOAD ROM PORT 1 ADDRESS
      SRC P4     ; SEND ROM PORT ADDRESS
      RDR        ; READ INPUT B
      XCH R1     ; B TO REGISTER 1
      JMS AND    ; EXECUTE "AND"
      XCH R2     ; LOAD RESULT C
      WMP        ; STORE AT MEMORY PORT 0
      JUN START  ; RESTART
      NOP

; "AND" subroutine
*=104
AND    CLB            ; CLEAR ACCUMULATOR AND CARRY
       XCH 2          ; CLEAR REGISTER 2
       LDM 4          ; LOAD LOOP COUNT (LC)
AND_3  XCH 0          ; LOAD A, LC TO REGISTER 0
       RAR            ; ROTATE LEAST SIGNIFICANT BIT TO CARRY
       XCH 0          ; RETURN ROTATED A TO REG 0, LC TO ACC
       JCN CZ, ROTR1  ; JUMP TO ROTR1 IF CARRY ZERO
       XCH 1          ; LOAD B, LC TO ACCUMULATOR
       RAR            ; ROTATE LEAST SIGNIFICANT BIT TO CARRY
       XCH 1          ; RETURN ROTATED B TO REG 1, LC TO ACC
ROTR2  XCH 2          ; LOAD PARTIAL RESULT C, LC TO REG 2
       RAR            ; ROTATE CARRY INTO PARTIAL RESULT MSB
       XCH 2          ; LOAD LC, RETURN C TO REGISTER 2
       DAC            ; DECREMENT THE ACCUMULATOR (LC)
       JCN ANZ, AND_3 ; LOOP IF LC NON ZERO
       BBL 0          ; RETURN
ROTR1  XCH 1          ; LOAD B, LC TO REGISTER 1
       RAR            ; ROTATE B
       XCH 1          ; RETURN ROTATED B TO REG 1, LC TO ACC
       CLC            ; CLEAR CARRY
       JUN ROTR2      ; RETURN TO LOOP

CZ=10
ANZ=12

$
