global _ft_strchr

section .text

_ft_strchr:
    mov rax, rdi

.find_char:
    mov dl, byte [rax]
    cmp dl, sil
    je .found

    cmp dl, 0
    je .not_found

    inc rax             ; advance to next character
    jmp .find_char

.found:
    ret

.not_found:
    xor rax, rax
    ret