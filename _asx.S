.intel_syntax noprefix

.data
W:
	.quad 64									# word size
D:
	.quad 4										# max number of digits
cf:
	.quad 0										# carry flag
bf:
	.quad 0										# borrow flag

.text

.global _xcf
_xcf:
	mov rax, cf
	ret
	
.global _xbf
_xbf:
	mov rax, bf
	ret

.global _xmov
_xmov:
	mov r10, 0									# r10 = 0
	xmov_for:
		cmp r10, D								# ? r10 < D ?
		jge end_xmov_for 						# if not -> end the loop
		mov rax, [rsi + 8 * r10]				# rax = regs[r10]
		mov [rdi + 8 * r10], rax				# regd[r10] = rax
		inc r10									# r10++
		jmp xmov_for
	end_xmov_for:
		ret

.global _xadd
_xadd:
	mov r10, 1
	
	mov rax, [rsi]								# rax = regs[0]
	add [rdi], rax								# regd[0] += rax
	
	mov rax, [rsi + 8 * r10]					# rax = regs[1]
	adc [rdi + 8 * r10], rax					# regd[1] += rax + carry
	inc r10
	
	mov rax, [rsi + 8  * r10]					# rax = regs[2]
	adc [rdi + 8 * r10], rax					# regd[2] += rax + carry
	inc r10
	
	mov rax, [rsi + 8 * r10]					# rax = regs[3]
	adc [rdi + 8 * r10], rax					# regd[3] += rax + carry
	
	mov r10, 0
	adc r10, 0									# r10 = carry flag
	mov cf, r10									# store carry flag
	
	ret
	
.global _xadc
_xadc:
	mov r10, 1
	mov r11, cf
	add r11, 0xFFFFFFFFFFFFFFFF					# restore carry flag
	
	mov rax, [rsi]								# rax = regs[0]
	adc [rdi], rax								# regd[0] += rax + carry
	
	mov rax, [rsi + 8 * r10]					# rax = regs[1]
	adc [rdi + 8 * r10], rax					# regd[1] += rax + carry
	inc r10
	
	mov rax, [rsi + 8  * r10]					# rax = regs[2]
	adc [rdi + 8 * r10], rax					# regd[2] += rax + carry
	inc r10
	
	mov rax, [rsi + 8 * r10]					# rax = regs[3]
	adc [rdi + 8 * r10], rax					# regd[3] += rax + carry
	
	mov r10, 0
	adc r10, 0									# r10 = carry flag
	mov cf, r10									# store carry flag
	
	ret
	
.global _xinc
_xinc:
	mov r10, 1
	
	mov r11, [rdi]
	mov r12, 0
	add r11, r10
	mov [rdi], r11 								# regd[0]++
	
	adc [rdi + 8 * r10], r12					# regd[1] += carry
	inc r10
	
	adc [rdi + 8 * r10], r12					# regd[2] += carry
	inc r10
	
	adc [rdi + 8 * r10], r12					# regd[3] += carry
	
	mov r10, 0
	adc r10, 0									# r10 = carry flag
	mov cf, r10									# store carry flag
	
	ret
	
.global _xsub
_xsub:
	mov r10, 1
	
	mov rax, [rsi]								# rax = regs[0]
	sub [rdi], rax								# regd[0] -= rax
	
	mov rax, [rsi + 8 * r10]					# rax = regs[1]
	sbb [rdi + 8 * r10], rax					# regd[1] -= rax - borrow
	inc r10
	
	mov rax, [rsi + 8  * r10]					# rax = regs[2]
	sbb [rdi + 8 * r10], rax					# regd[2] -= rax - borrow
	inc r10
	
	mov rax, [rsi + 8 * r10]					# rax = regs[3]
	sbb [rdi + 8 * r10], rax					# regd[3] -= rax - borrow
	
	mov r10, 0
	adc r10, 0									# r10 = borrow flag
	mov bf, r10									# store borrow flag
	
	ret
	
.global _xsbb
_xsbb:
	mov r10, 1
	mov r11, bf
	mov r12, 0
	sub r12, r11								# restore borrow flag
	
	mov rax, [rsi]								# rax = regs[0]
	sbb [rdi], rax								# regd[0] -= rax - borrow
	
	mov rax, [rsi + 8 * r10]					# rax = regs[1]
	sbb [rdi + 8 * r10], rax					# regd[1] -= rax - borrow
	inc r10
	
	mov rax, [rsi + 8  * r10]					# rax = regs[2]
	sbb [rdi + 8 * r10], rax					# regd[2] -= rax - borrow
	inc r10
	
	mov rax, [rsi + 8 * r10]					# rax = regs[3]
	sbb [rdi + 8 * r10], rax					# regd[3] -= rax - borrow
	
	mov r10, 0
	adc r10, 0									# r10 = borrow flag
	mov bf, r10									# store borrow flag
	
	ret
	
.global _xdec
_xdec:
	mov r10, 1
	
	mov r11, [rdi]
	mov r12, 0
	sub r11, r10
	mov [rdi], r11 								# regd[0]--
	
	sbb [rdi + 8 * r10], r12					# regd[1] -= borrow
	inc r10
	
	sbb [rdi + 8 * r10], r12					# regd[2] -= borrow
	inc r10
	
	sbb [rdi + 8 * r10], r12					# regd[3] -= borrow
	
	mov r10, 0
	sbb r10, 0									# r10 = borrow flag
	mov bf, r10									# store borrow flag
	
	ret
	
.global _xmul
_xmul:
	mov r10, -1									# r10 = i = -1
	mov r12, 0									# r12 = 0
	mov rbx, D									# rbx = D
	lea r14, [rdx]								# r14 = &regs1
	lea r15, [rcx]								# r15 = &regs2

	loop1:
		inc r10									# i++
		cmp r10, rbx							# ? i < al ?
		jge _xmul_1								# if false
		mov [rdi + 8*r10], r12					# regd_lo[i] = 0
		mov [rsi + 8*r10], r12					# regd_hi[i] = 0
		jmp loop1

	_xmul_1:
		mov r10, -1								# r10 = i = 0
	
	loop2:
		inc r10									# i++
		cmp r10, rbx							# ? i < al ?
		jge exit
		mov r13, 0								# r13 = U = 0
		mov r11, -1								# r11 = j = -1
	
	loop3:
		inc r11									# j++
		cmp r11, rbx							# ? j < al ?
		jge _xmul_3
		mov rax, [r14 + 8*r10]					# rax = a[i]
		mov rcx, [r15 + 8*r11]					# rcx = b[j]
		mov r12, r10							# r12 = i
		add r12, r11							# r12 = i + j
		cmp r12, D
		jge indexing
		mov r12, [rdi + 8*r12]					# r12 = z_lo[i + j]
		jmp indexing_end
		indexing:
			sub	r12, D
			mov r12, [rsi + 8*r12]				# r12 = z_hi[i + j - D]					
		indexing_end:
		
		xor rdx, rdx							# rdx = 0
		mul rcx									# rdx|rax = a[i] * b[j] 
		add rax, r13							# rax += U
		adc rdx, 0								# rdx += carry
		adc rax, r12							# rax += r12 + carry
		adc rdx, 0								# rdx = (a[i] * b[j] >> 64) + carry
		mov r13, rdx							# U = rdx
		mov r12, r10
		add r12, r11
		cmp r12, D
		jge indexing2
		mov [rdi + 8*r12], rax					# r12 = z_lo[i + j]
		jmp indexing2_end
		indexing2:
			sub	r12, D
			mov [rsi + 8*r12], rax				# z_hi[i + j - D] = V
		indexing2_end:
		jmp loop3
	
	_xmul_3:
		mov r11, r10							# r11 = i
		mov [rsi + 8*r11], rdx					# z_hi[i] = V
		jmp loop2

	exit:
		ret
	
.global _xshr
_xshr:
	mov r10, 1
	mov r15, 0

	mov r11, [rsi]								# r11 = sh
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi]
	shr rax, cl									# regs[0] >>= sh
	mov [rdi], rax
	
	mov r12, W									# r12 = W
	sub r12, r11								# r12 = W - sh
	mov rcx, r12								# cl = r12 = W - sh

	mov r13, [rdi + 8 * r10]					# r13 = regs[1]
	shl r13, cl									# r13 = a = regs[1] << (W - sh)
	shr r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shr rax, cl									# regs[1] >>= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12								# cl = r12 = W - sh
	shl r14, cl									# r14 = a << (W - sh)
	or [rdi + 8 * r15], r14						# regs[0] | (r14)
	inc r10
	inc r15
	
	mov r13, [rdi + 8 * r10]					# r13 = regs[2]
	shl r13, cl									# r13 = a = regs[2] << (W - sh)
	shr r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shr rax, cl									# regs[2] >>= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12								# cl = r12 = W - sh
	shl r14, cl									# r14 = a << (W - sh)
	or [rdi + 8 * r15], r14						# regs[2] | (r14)
	inc r10
	inc r15
	
	mov r13, [rdi + 8 * r10]					# r13 = regs[3]
	shl r13, cl									# r13 = a = regs[3] << (W - sh)
	shr r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shr rax, cl									# regs[3] >>= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12									# cl = r12 = W - sh
	shl r14, cl									# r14 = a << (W - sh)
	or [rdi + 8 * r15], r14						# regs[3] | (r14)
	
	ret
	
.global _xshl
_xshl:
	mov r10, D
	sub r10, 2
	mov r15, D
	sub r15, 1

	mov r11, [rsi]								# r11 = sh
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r15]
	shl rax, cl									# regs[3] <<= sh
	mov [rdi + 8 * r15], rax
	
	mov r12, W									# r12 = W
	sub r12, r11								# r12 = W - sh
	mov rcx, r12								# cl = r12 = W - sh

	mov r13, [rdi + 8 * r10]					# r13 = regs[2]
	shr r13, cl									# r13 = a = regs[2] >> (W - sh)
	shl r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shl rax, cl									# regs[2] <<= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12								# cl = r12 = W - sh
	shr r14, cl									# r14 = a >> (W - sh)
	or [rdi + 8 * r15], r14						# regs[3] | (r14)
	dec r10
	dec r15
	
	mov r13, [rdi + 8 * r10]					# r13 = regs[1]
	shr r13, cl									# r13 = a = regs[1] >> (W - sh)
	shl r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shl rax, cl									# regs[1] <<= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12								# cl = r12 = W - sh
	shr r14, cl									# r14 = a >> (W - sh)
	or [rdi + 8 * r15], r14						# regs[2] | (r14)
	dec r10
	dec r15
	
	mov r13, [rdi + 8 * r10]					# r13 = regs[0]
	shr r13, cl									# r13 = a = regs[0] >> (W - sh)
	shl r13, cl
	mov rcx, r11								# cl = r11 = sh
	mov rax, [rdi + 8 * r10]
	shl rax, cl									# regs[0] <<= sh
	mov [rdi + 8 * r10], rax
	mov r14, r13								# r14 = a
	mov rcx, r12								# cl = r12 = W - sh
	shr r14, cl									# r14 = a >> (W - sh)
	or [rdi + 8 * r15], r14						# regs[1] | (r14)
	
	ret
	
.global _xpush
_xpush:
	pop rax										# rax = rip
	mov r10, 0									# r10 = 0
	xpush_for:
		cmp r10, D								# ? r10 < D ?
		jge end_xpush_for 						# if not -> end the loop
		push [rdi + 8 * r10]					# push
		inc r10									# r10++
		jmp xpush_for
	end_xpush_for:
		push rax								# push rax = rip
		ret
	
.global _xpop
_xpop:
	pop rax										# rax = rip
	mov r10, D									# r10 = D
	sub r10, 1									# r10 = D - 1
	xpop_for:
		cmp r10, D								# ? r10 >= 0 ?
		jl end_xpop_for 						# if not -> end the loop
		pop [rdi + 8 * r10]						# pop
		dec r10									# r10--
		jmp xpop_for
	end_xpop_for:
		push rax								# push rax = rip
		ret
	
#.global _xlbl
#_xlbl:
#	pop r10										# rax = rip
#	pop r11										# rax = rip
#	pop r12										# rax = rip
#	pop r13										# rax = rip
#	pop r14										# rax = rip
#	
#	mov rax, r14
#	sub rax, 20	
#	
#	push r14									# push rip back
#	push r13									# push rip back
#	push r12									# push rip back
#	push r11									# push rip back
#	push r10									# push rip back
#	
#	ret											# return rip
#	
#.global _xjmp
#_xjmp:
#	jmp rdi
	
	
	
	
	
	
	
	
	
	
	