global ft_list_size


; Counts the number of nodes in a linked list
;
; Prototype: int ft_list_size(t_list *begin_list)
;
; Arguments:
;   rdi = begin_list (t_list *) - pointer to the head node
;
; Returns:
;   rax = number of nodes in the list
;
; t_list struct layout (16 bytes total):
;   offset 0:  data  (qword - 8 bytes)
;   offset 8:  next  (qword - 8 bytes)

ft_list_size:
    xor     eax, eax ; count = 0

.loop:
    test    rdi, rdi ; if (current == NULL)
    jz      .done

    inc     rax     ; count++
    mov     rdi, [rdi + 8] ; current = current->next (offset 8)

    jmp     .loop

.done:
    ret

