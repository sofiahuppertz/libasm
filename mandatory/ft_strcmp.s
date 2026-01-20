global ft_strcmp

section .text

ft_strcmp:
.loop:
    mov     al, [rdi]           ; al = *s1
    mov     cl, [rsi]           ; cl = *s2 (use cl, not bl - rbx is callee-saved!)
    cmp     al, cl
    jne     .diff
    test    al, al              ; if *s1 == '\0'
    je      .equal
    inc     rdi
    inc     rsi
    jmp     .loop
.diff:
    movzx   eax, al
    movzx   ecx, cl
    sub     eax, ecx            ; return *s1 - *s2
    ret
.equal:
    xor     eax, eax            ; return 0
    ret
