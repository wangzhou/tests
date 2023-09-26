// sf op S        sh imm12        rn    rd
// 1  0  0 100010 0  xxxxxxxxxxxx xxxxx xxxxx
//
//                   imm12: 2     x10   x12
// 1  0  0 100010 0  000000000010 01010 01100  ---+
//                                                |
// 10010001000000000000100101001100         <-----+
.text
.global add2
add2:
	mov x10, x0
.word   0b10010001000000000000100101001100
        mov x0, x12
	ret

