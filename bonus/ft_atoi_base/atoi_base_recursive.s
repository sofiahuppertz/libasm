global _recursive_atoi

extern _ft_strlen
extern _get_digit_value

section .text

; rdi = base
; rsi = str

_recursive_atoi:

    cmp     byte[rsi], 0
    je      .ret_zero

    push    rbx
    push    r12

    mov     rbx, rdi            ; rbx = base
    mov     r12, rsi            ; r12 = str

    call    _ft_strlen
    push    r13
    mov     r13, rax            ; r13= base_len

    mov     rsi, r12
    cmp     byte[rsi + 1], 0
    jne     .recurse_case

    mov     dil, byte[rsi]
    mov     rsi, rbx
    call    _get_digit_value

    pop     r13
    pop     r12
    pop     rbx

    ret

.recurse_case:
    mov rdx, r12

.find_end:
    cmp     byte[rdx + 1], 0
    je      .have_end
    inc     rdx
    jmp     .find_end

.have_end:
    push    r14
    movzx     r14, byte[rdx]
    mov     byte[rdx], 0

    mov     rdi, rbx
    mov     rsi, r12
    call    _recursive_atoi

    imul    r13, rax
    mov     byte[rdx], r14

    mov     dil, r14
    mov     rsi, rbx
    call    _get_digit_value

    add     rax, r13

    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret

.ret_zero:
    xor     eax, eax
    ret




