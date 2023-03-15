global main
extern strlen


section .rodata
	linedown: db 0xA, 0
	
section .text
main:
	push ebp		; start the program
	mov ebp, esp	
	pushad

	mov esi, [ebp+8]	;esi = argc  edx is uor flag
	mov eax,[ebp+12]	;eax=argv[0]
	add edx, 0
	jz end_loop		;check if argc>0
start_loop:
	
	pushad
	mov ecx, [eax]	;prepare argv[i]
	mov eax, 4
	mov ebx, 1
	mov edx, 8 ; edx = len
	int 0x80
	popad

	pushad
	mov ecx, linedown	;prepare argv[i]
	mov eax, 4
	mov ebx, 1
	mov edx, 1 ; 
	int 0x80
	popad
	
	add eax, 4		;continue to next element
	dec esi			;decrese the flag 
	jnz start_loop		; if flag 1= 0 go back to start_loop
end_loop:
	
	popad			;end the program 
	mov esp, ebp	
	pop ebp
	mov eax, 0		;return 0
	int 0x80

