global _get_digit_value

section .text

; dil is the char
; rsi is the pointer to base str

_get_digit_value:
    mov rax, rsi

.next_char:
    cmp byte[rax], 0
    je .not_found

    cmp byte[rax], dil
    je .found

    inc rax
    jmp .next_char

.found:
    sub rax, rsi
    ret

.not_found:
    mov eax, -1
    ret