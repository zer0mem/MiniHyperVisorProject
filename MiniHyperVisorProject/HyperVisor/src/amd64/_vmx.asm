.code

get_guest_exit proc
	lea rax, [rsp + sizeof(QWORD)]
	mov [rcx], rax
	
	mov rax, [rsp]
	mov [rdx], rax
		
	xor rax, rax	
	ret
get_guest_exit endp

end