    global ft_strdup

    extern ft_strlen
    extern ft_strcpy
    extern malloc

    section .text

ft_strdup:
    push rdi             
    call ft_strlen    
    inc rax       
    mov rdi, rax      
    call malloc           
    test rax, rax
    je .malloc_fail
    pop rsi               
    mov rdi, rax         
    call ft_strcpy
    ret

.malloc_fail:
    xor eax, eax
    ret