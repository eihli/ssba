	section .text

global pangram
pangram:
	;; Use bits to keep track of which letters we've seen.
	;; 'A' is character 65 in ASCII
	;; 'z' is character 122 in ASCII
	;; A 57 bit number can keep track of every character.
	;;
	;; Initialize a counter in RCX to loop through the string
	;; and RAX to the bits to track which characters we've seen.
	xor rcx, rcx
	mov rax, (1 << 58) - 1
loop:
	cmp byte [rdi + rcx], 0
	je end
	movzx r8, byte [rdi + rcx]
	;; Whatever character we see, subtract 'A' from it so
	;; that 'A' is 0, 'B', is 1, ..., and 'z' is 57.
	sub r8, 65
	mov r9, 1
iter:
	;; Position the bit representing the character.
	;; Then zero out that bit in RAX.
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
	;; Save off RAX to R10 temporarily so we can shift it right for lowercase
	;; Now we've got occurrences of 'A' through 'Z' in RAX
	;; and occurrences of 'a' through' 'z' in R10.
	;;
	;; If there's still a 1 in both the lowercase AND the uppercase bits,
	;; then we haven't seen either, so this function should return FALSE.
	mov r10, rax
	shr r10, 32
	and rax, r10
	;; Zero out everything above 26th bit. This makes it so we ignore the characters
	;; between 'Z' and 'a'.
	and rax, (1 << 27) - 1
	cmp rax, 0
	je true
	mov rax, 0
	ret
true:
	mov rax, 1
	ret
