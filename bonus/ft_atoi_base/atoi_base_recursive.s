global recursive_atoi

extern ft_strlen
extern get_digit_value

section .text

; Converts a string to integer using a custom base (recursive implementation)
; Arguments:
;   rdi = base string (e.g., "0123456789" for decimal)
;   rsi = str to convert
; Returns:
;   rax = integer value
; Callee-saved registers used: rbx, r12, r13, r14, r15

recursive_atoi:

    ; Base case: empty string returns 0
    cmp     byte[rsi], 0
    je      .ret_zero

    ; Save callee-saved registers
    push    rbx
    push    r12

    mov     rbx, rdi            ; rbx = base (preserved across calls)
    mov     r12, rsi            ; r12 = str (preserved across calls)

    ; Get base length (e.g., 10 for decimal, 16 for hex)
    call    ft_strlen          ; rdi still contains base
    push    r13
    mov     r13, rax            ; r13 = base_len

    ; Check if single character (base case for recursion)
    mov     rsi, r12
    cmp     byte[rsi + 1], 0
    jne     .recurse_case

    ; Single char: just return its digit value
    movzx   edi, byte[rsi]      ; edi = character (zero-extended)
    mov     rsi, rbx            ; rsi = base
    call    get_digit_value    ; rax = digit value of the character

    pop     r13
    pop     r12
    pop     rbx

    ret

.recurse_case:
    ; Find pointer to last character
    mov     rdx, r12

.find_end:
    cmp     byte[rdx + 1], 0    ; is next char null?
    je      .have_end
    inc     rdx
    jmp     .find_end

.have_end:
    ; Save more callee-saved registers
    push    r14
    push    r15

    ; Save last char and its position, then truncate string
    movzx   r14, byte[rdx]      ; r14 = last character (zero-extended)
    mov     r15, rdx            ; r15 = pointer to last char (rdx is volatile)
    mov     byte[rdx], 0        ; temporarily null-terminate (remove last char)

    ; Recurse on shorter string: atoi(base, str_without_last_char)
    mov     rdi, rbx
    mov     rsi, r12
    call    recursive_atoi     ; rax = value of str[0..n-1]

    ; result = recursive_result * base_len
    imul    r13, rax            ; r13 = recursive_result * base_len

    ; Restore the last character
    mov     byte[r15], r14b

    ; Get digit value of last character
    movzx   edi, r14b           ; edi = last char (zero-extended)
    mov     rsi, rbx            ; rsi = base
    call    get_digit_value    ; rax = digit value of last char

    ; Final result = (recursive_result * base_len) + last_digit
    add     rax, r13

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret

.ret_zero:
    xor     eax, eax
    ret




