
test:     file format elf64-littleriscv


Disassembly of section .note.gnu.build-id:

0000000000010120 <.note.gnu.build-id>:
   10120:	0004                	0x4
   10122:	0000                	unimp
   10124:	0014                	0x14
   10126:	0000                	unimp
   10128:	00000003          	lb	zero,0(zero) # 0 <add-0x10144>
   1012c:	00554e47          	fmsub.s	ft8,fa0,ft5,ft0,rmm
   10130:	9cf2                	add	s9,s9,t3
   10132:	5101                	li	sp,-32
   10134:	d075                	beqz	s0,10118 <add-0x2c>
   10136:	6cbc                	ld	a5,88(s1)
   10138:	db3c                	sw	a5,112(a4)
   1013a:	007e                	c.slli	zero,0x1f
   1013c:	1326                	slli	t1,t1,0x29
   1013e:	08be01d7          	0x8be01d7
   10142:	02df        	0x2df

Disassembly of section .text:

0000000000010144 <add>:
   10144:	7179                	addi	sp,sp,-48
   10146:	f422                	sd	s0,40(sp)
   10148:	1800                	addi	s0,sp,48
   1014a:	87aa                	mv	a5,a0
   1014c:	872e                	mv	a4,a1
   1014e:	fcf42e23          	sw	a5,-36(s0)
   10152:	87ba                	mv	a5,a4
   10154:	fcf42c23          	sw	a5,-40(s0)
   10158:	fdc42703          	lw	a4,-36(s0)
   1015c:	fd842783          	lw	a5,-40(s0)
   10160:	9fb9                	addw	a5,a5,a4
   10162:	fef42623          	sw	a5,-20(s0)
   10166:	fec42783          	lw	a5,-20(s0)
   1016a:	853e                	mv	a0,a5
   1016c:	7422                	ld	s0,40(sp)
   1016e:	6145                	addi	sp,sp,48
   10170:	8082                	ret

0000000000010172 <main>:
   10172:	1101                	addi	sp,sp,-32
   10174:	ec06                	sd	ra,24(sp)
   10176:	e822                	sd	s0,16(sp)
   10178:	e426                	sd	s1,8(sp)
   1017a:	1000                	addi	s0,sp,32
   1017c:	4085                	li	ra,1
   1017e:	408d                	li	ra,3
   10180:	4491                	li	s1,4
   10182:	00001797          	auipc	a5,0x1
   10186:	05e7b783          	ld	a5,94(a5) # 111e0 <_GLOBAL_OFFSET_TABLE_+0x8>
   1018a:	439c                	lw	a5,0(a5)
   1018c:	2785                	addiw	a5,a5,1
   1018e:	2781                	sext.w	a5,a5
   10190:	80be                	mv	ra,a5
   10192:	0089                	addi	ra,ra,2
   10194:	00308493          	addi	s1,ra,3
   10198:	0000879b          	sext.w	a5,ra
   1019c:	0004871b          	sext.w	a4,s1
   101a0:	85ba                	mv	a1,a4
   101a2:	853e                	mv	a0,a5
   101a4:	fa1ff0ef          	jal	ra,10144 <add>
   101a8:	87aa                	mv	a5,a0
   101aa:	873e                	mv	a4,a5
   101ac:	00001797          	auipc	a5,0x1
   101b0:	0347b783          	ld	a5,52(a5) # 111e0 <_GLOBAL_OFFSET_TABLE_+0x8>
   101b4:	c398                	sw	a4,0(a5)
   101b6:	4781                	li	a5,0
   101b8:	853e                	mv	a0,a5
   101ba:	60e2                	ld	ra,24(sp)
   101bc:	6442                	ld	s0,16(sp)
   101be:	64a2                	ld	s1,8(sp)
   101c0:	6105                	addi	sp,sp,32
   101c2:	8082                	ret

Disassembly of section .got:

00000000000111c8 <.got>:
   111c8:	ffff                	0xffff
   111ca:	ffff                	0xffff
   111cc:	ffff                	0xffff
   111ce:	ffff                	0xffff
	...

00000000000111d8 <_GLOBAL_OFFSET_TABLE_>:
	...
   111e0:	11e8                	addi	a0,sp,236
   111e2:	0001                	nop
   111e4:	0000                	unimp
	...

Disassembly of section .bss:

00000000000111e8 <d>:
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	3a434347          	fmsub.d	ft6,ft6,ft4,ft7,rmm
   4:	2820                	fld	fs0,80(s0)
   6:	6255                	lui	tp,0x15
   8:	6e75                	lui	t3,0x1d
   a:	7574                	ld	a3,232(a0)
   c:	3920                	fld	fs0,112(a0)
   e:	342e                	fld	fs0,232(sp)
  10:	302e                	fld	ft0,232(sp)
  12:	312d                	addiw	sp,sp,-21
  14:	6275                	lui	tp,0x1d
  16:	6e75                	lui	t3,0x1d
  18:	7574                	ld	a3,232(a0)
  1a:	7e31                	lui	t3,0xfffec
  1c:	3032                	fld	ft0,296(sp)
  1e:	302e                	fld	ft0,232(sp)
  20:	2934                	fld	fa3,80(a0)
  22:	3920                	fld	fs0,112(a0)
  24:	342e                	fld	fs0,232(sp)
  26:	302e                	fld	ft0,232(sp)
	...
