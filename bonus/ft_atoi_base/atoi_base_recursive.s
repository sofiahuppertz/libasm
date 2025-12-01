global _recursive_atoi

extern _ft_strlen
extern _get_digit_value

section .text

; rdi = base
; rsi = str

_recursive_atoi:

    cmp     byte[rsi], 0
    je      .ret_zero

    push    rdi
    push    rsi
    call    _ft_strlen  ; rax contains the len of base

    cmp     byte[rsi + 1], 0
    je      .base_case

    ; truncate last digit
    mov rdx, rsi

.find_end:
    cmp     byte[rdx + 1], 0
    je      .done
    inc     rdx
    jmp     .find_end

.done
    mov     r8b, byte[rdx] ; temporarily truncate
    mov     byte[rdx], 0
    pop      rsi
    pop      rdi
    push     r8b ; last char of str
    push     rdi;
    push     rax ; base lenght


    call    _recursive_atoi
    pop rdi
    mul rax, rdi
    pop rsi
    pop rdi
    push rax

    call _get_digit_value
    pop rsi
    add rax, rsi

    ret

.base_case:
    call _get_digit_value
    ret

.ret_zero:
    xor eax, eax
    ret









