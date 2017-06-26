[bits 64]
    global _start

    section .data
    message db "hello, asm. i love it.", 0xA
    .length equ $ - message

    section .text
_start:
    mov rax, 1
    mov rdx, message.length
    mov rsi, message
    mov rdi, 1
    syscall

    mov rax, 60 
    mov rdi, 0
    syscall
