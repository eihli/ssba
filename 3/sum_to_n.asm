section .text
global sum_to_n
;; sum_to_n:
;; 	mov eax, 0
;; begin:
;; 	 rdi, 0
;; 	je end
;; 	add rax, rdi
;; 	dec rdi
;; 	jmp begin
;; end:
;; 	ret

sum_to_n:
	mov rax, rdi
	inc rax
	imul rax, rdi
	shr rax, 1
	ret
