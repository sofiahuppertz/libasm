global ft_list_sort

section .text

; Sorts a linked list in ascending order using bubble sort
;
; Prototype: void ft_list_sort(t_list **begin_list, int (*cmp)())
;
; Arguments:
;   rdi = begin_list (t_list **) - pointer to the head pointer
;   rsi = cmp (function pointer)  - comparison function: cmp(data1, data2)
;                                   returns > 0 if data1 > data2
;
; t_list struct layout (16 bytes total):
;   offset 0:  data  (qword - 8 bytes)
;   offset 8:  next  (qword - 8 bytes)
;
; Register usage:
;   rbx = begin_list (t_list **)
;   r12 = cmp function pointer
;   r13 = current node pointer
;   r14 = next node pointer
;   r15 = swapped flag (0 = false, 1 = true)

ft_list_sort:
    ; Save callee-saved registers
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    mov     rbx, rdi            ; rbx = begin_list
    mov     r12, rsi            ; r12 = cmp function pointer

    ; ========== EDGE CASE CHECKS ==========

    test    rbx, rbx            ; if begin_list == NULL
    jz      .done               ;     nothing to sort

    mov     r13, [rbx]          ; r13 = *begin_list (head node)
    test    r13, r13            ; if list is empty
    jz      .done               ;     nothing to sort

    ; ========== BUBBLE SORT OUTER LOOP ==========
    ; Repeat until no swaps occur in a full pass

.outer_loop:
    xor     r15, r15            ; swapped = false
    mov     r13, [rbx]          ; current = *begin_list (start from head)

    ; ========== INNER LOOP: TRAVERSE LIST ==========
    ; Compare adjacent nodes and swap if needed

.inner_loop:
    mov     r14, [r13 + 8]      ; next = current->next
    test    r14, r14            ; if next == NULL
    jz      .check_swapped      ;     end of list, check if we need another pass

    ; Compare current->data with next->data
    mov     rdi, [r13]          ; arg1 = current->data
    mov     rsi, [r14]          ; arg2 = next->data
    call    r12                 ; result = cmp(current->data, next->data)

    cmp     eax, 0              ; if cmp() > 0
    jg      .swap               ;     elements are out of order, swap them

.advance:
    mov     r13, r14            ; current = next (move to next node)
    jmp     .inner_loop

    ; ========== CHECK IF ANOTHER PASS NEEDED ==========

.check_swapped:
    test    r15, r15            ; if swapped == true
    jnz     .outer_loop         ;     do another pass

    ; ========== CLEANUP AND RETURN ==========

.done:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret

    ; ========== SWAP DATA BETWEEN TWO NODES ==========

.swap:
    mov     rdi, [r13]          ; tmp = current->data
    mov     rsi, [r14]          ; (use rsi as temp holder)
    mov     [r13], rsi          ; current->data = next->data
    mov     [r14], rdi          ; next->data = tmp
    mov     r15, 1              ; swapped = true
    jmp     .advance            ; continue traversing