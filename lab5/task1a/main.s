global main
extern strlen
extern positive_atoi

section .rodata
	decimal: db "%d", 10, 0 ; "%d\n"
	is_e: db "+e"	
section .bss
	encry: resb 100
	len_encry: resb 4
	len_input: resb 4
	input: resb 100	
	output: resb 100
section .text

main:
	push ebp		; start the program
	mov ebp, esp	

	mov eax, 3		; sys_read system call command                                
	mov ebx, [ebp+16]	
	mov ecx, encry
	mov edx, 100
	int 0x80
	mov [len_encry], eax
	

	mov eax, 3		; sys_read system call stdin                                
	mov ebx, 2	
	mov ecx, input	
	mov edx, 100
	int 0x80
	mov [len_input], eax

	mov eax, encry			
	cmp dword[eax], '+'+(256*'e') 	; if not +e
	jnz zibi

	mov eax, encry
	add eax , byte 2
	mov [encry], eax 
	push encry  			 ; swap encry to int
	call positive_atoi
	add esp, 4
	mov [encry], eax
	mov eax, 4
	mov ebx, 1
	mov ecx, encry
	mov edx, 5
	int 0x80 

	mov esi , [len_input-1]	;counter the len of the input
	mov ecx, input

swaploop:
			
	mov byte bl , [eax]		; first byte of encry
	mov byte dl , [ecx]		; first byte of input
	add bl ,dl				; sum bytes
	mov byte [ecx] ,bl		; input[i] = bl
	inc ecx					; input one byte forward
	inc eax					; encry one byte forward
	dec esi 				; counter --
	jnz swaploop

	push input
	call strlen
	add esp, 4 
	mov [len_input], eax

zibi:
	mov eax, 4
	mov ebx, 1
	mov ecx, input
	mov edx, [len_input-1]
	int 0x80 

    mov eax, 1
	mov ebx, 0
    int 0x80
