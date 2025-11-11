global _ft_strcmp

section .text

_ft_strcmp:
.loop:
    mov al, [rdi]
    mov bl, [rsi]
    cmp al, bl
    jne .diff
    test al, al
    je .equal
    inc rdi
    inc rsi
    jmp .loop
.diff:
    movzx eax, al
    movzx ebx, bl
    sub eax, ebx
    ret
.equal:
    xor eax, eax
    ret
