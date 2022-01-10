	section .text

global pangram
pangram:
	xor rcx, rcx
	mov rax, (1 << 58) - 1
loop:
	cmp byte [rdi + rcx], 0
	je end
	;; Get current character.
	;; Subtract 0x61
	;; Shift left by rank
	movzx r8, byte [rdi + rcx]
	sub r8, 65
	mov r9, 1
iter:
	cmp r8, 0
	jle done
	shl r9, 1
	dec r8
	jmp iter
done:
	not r9
	and rax, r9
	inc rcx
	jmp loop
end:
	;; Save off rax temporarily so we can shift it right for lowercase
	mov r10, rax
	shr r10, 32
	and rax, r10
	;; Zero out everything above 26th bit
	and rax, (1 << 27) - 1
	cmp rax, 0
	je true
	mov rax, 0
	ret
true:
	mov rax, 1
	ret
