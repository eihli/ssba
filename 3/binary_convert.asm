section .text
global binary_convert
binary_convert:
	mov rax, 0 					; result accumulator
	mov rcx, 0					; index counter to iterate over string
loop:
	cmp byte[rdi + rcx], 0x00	; at null-termination char?
	je end
	shl rax, 1
	xor rdx, rdx 				; zero out for clean add to rax
	mov dl, [rdi + rcx]
	sub dl, 0x30
	add rax, rdx
	inc rcx
	jmp loop
end:
	ret
