section .data
    state dw 0       ; seed for the random number generator

section .text
    global main

main:
    ; initialize the random number generator with a seed value
    mov ax, state
    mov cx, 0123h    ; arbitrary value for the seed
    xor ax, cx
    mov state, ax

    ; generate a random number and store it in bx
    call rand
    mov bx, ax

    ; do something with the random number in bx

    ; generate another random number and store it in cx
    call rand
    mov cx, ax

    ; do something with the random number in cx

    ; exit the program
    mov ax, 0x4C00
    int 0x21

; generates a random number and stores it in ax
rand:
    ; rotate the bits of the state variable to the right
    mov cl, 1
    ror state, cl

    ; xor the low and high bytes of the state variable
    mov ax, state
    xor al, ah
    mov state, ax

    ; return the random number in ax
    ret
