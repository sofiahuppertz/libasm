global _ft_atoi_base

extern  _ft_strlen
extern  _ft_strchr
extern  _has_duplicates
extern  _ft_iswhitespace
extern  _recursive_atoi

section .text

; Converts a string to integer using a custom base
; Arguments:
;   rdi = str (string with number to convert)
;   rsi = base (e.g., "0123456789" for decimal)
; Returns:
;   rax = converted integer, or 0 if invalid
; Callee-saved registers used: rbx, r12, r13, r14

_ft_atoi_base:
    ; Save callee-saved registers
    push    rbx
    push    r12
    push    r13
    push    r14

    mov     rbx, rdi            ; rbx = str (preserved across calls)
    mov     r12, rsi            ; r12 = base (preserved across calls)

    ; ========== BASE VALIDATION ==========

    ; if (ft_strlen(base) < 2) return 0
    mov     rdi, r12
    call    _ft_strlen
    cmp     rax, 2
    jb      .ret_zero

    ; if (ft_strchr(base, '-')) return 0 (base can't contain sign chars)
    mov     rdi, r12
    mov     rsi, '-'
    call    _ft_strchr
    test    rax, rax
    jnz     .ret_zero

    ; if (ft_strchr(base, '+')) return 0
    mov     rdi, r12
    mov     rsi, '+'
    call    _ft_strchr
    test    rax, rax
    jnz     .ret_zero

    ; if (has_duplicates(base)) return 0 (each digit must be unique)
    mov     rdi, r12
    call    _has_duplicates
    test    rax, rax
    jnz     .ret_zero

    ; ========== SKIP WHITESPACE & PARSE SIGN ==========

    mov     r13, 1              ; r13 = sign multiplier (1 or -1)

.find_sign:
    ; Check for minus sign
    cmp     byte[rbx], '-'
    je      .negate

    ; Check for plus sign
    cmp     byte[rbx], '+'
    je      .increase_and_loop

    ; Check for whitespace
    movzx   edi, byte[rbx]
    call    _ft_iswhitespace
    test    rax, rax
    jnz     .increase_and_loop

    ; ========== VALIDATE STRING CHARACTERS ==========

    mov     r14, rbx            ; r14 = pointer to walk through str

.find_invalid_char:
    ; Check if we reached end of string
    cmp     byte[r14], 0
    je      .str_is_valid

    ; Check if current char exists in base
    mov     rdi, r12            ; base
    movzx   esi, byte[r14]      ; current character
    call    _ft_strchr
    test    rax, rax
    jz      .ret_zero           ; char not in base = invalid

    inc     r14
    jmp     .find_invalid_char

    ; ========== CONVERT STRING TO INTEGER ==========

.str_is_valid:
    mov     rdi, r12            ; base
    mov     rsi, rbx            ; str (after whitespace/signs)
    call    _recursive_atoi
    imul    rax, r13            ; apply sign: result = number * sign

.done:
    ; Restore callee-saved registers and return
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret

.increase_and_loop:
    inc     rbx                 ; move to next character
    jmp     .find_sign

.negate:
    imul    r13, -1             ; flip sign
    jmp     .increase_and_loop

.ret_zero:
    xor     eax, eax            ; return 0
    jmp     .done
