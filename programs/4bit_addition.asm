; add two 4bit numbers on the Intel 4004

    FIM P0, $A2	; initialize: R0=2 R1=A
    LD R0       ; load R0 into accumulator
    ADD R1      ; add R1 into accumulator
    XCH R1      ; and store in R1

DONE
    JUN DONE    ; endless loop as end of program
