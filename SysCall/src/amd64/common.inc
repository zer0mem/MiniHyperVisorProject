pushaq macro	
	push r8
	mov r8, rsp
	add r8, sizeof(qword) ;LOAD RSP

	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	push rax
	push rcx
	push rdx
	push rbx
	push r8 ;PUSH RSP
	mov r8, [r8 - sizeof(qword)]
	push rbp
	push rsi
	push rdi
endm

popaq macro
	pop rdi
	pop rsi
	pop rbp
	add rsp, sizeof(qword) ;SKIP POP RSP
	pop rbx
	pop rdx
	pop rcx
	pop rax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
endm

pushsq macro
	mov rax,cs
	push rax
	mov rax,ds
	push rax
	mov rax,es
	push rax
	mov rax,ss
	push rax

	push fs
	push gs
endm

;push / pop volatile register except rax (cause it is return val)
pushv macro
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
endm

popv macro
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
endm

pushptr macro
	sub rsp, sizeof(qword)
endm

popptr macro
	add rsp, sizeof(qword)
endm

xpop macro
	pop rax
	xchg [rsp],rax
endm

popsq macro
	pop gs
	pop fs

	push rax
	xpop
	mov ss,rax
	xpop
	mov es,rax
	xpop
	mov ds,rax
	xpop
	mov cs,rax
endm

pushrip macro
	local _rip
	call _rip
_rip:
endm

ENTER_HOOK_PROLOGUE macro
	push rax ; jmp addr {ret}
endm

ENTER_HOOK macro hook
	pushfq
	pushaq

	mov rcx, rsp
	pushptr
	call hook
	popptr

	;mov [rsp + 07h * sizeof(qword)], rax
	mov [rsp + (010h + 1) * sizeof(qword)], rax
	popaq
	popfq
	xchg [rsp], rax;bullshit due fix, TODO, remove + fix usage rax after ENTER_HOOK !!
endm

ENTER_HOOK_EPILOGUE macro
	xchg [rsp], rax ; final set ret addr to jmp -> original hooked fnctn
endm

.data

MAGIC							equ		04C495052h
IA32_SYSENTER_EIP				equ		0176h
IA64_SYSENTER_EIP				equ		0C0000082h
Ring3RSP						equ		010h
Ring0RSP						equ		01a8h
VMX_VMCS_HOST_SYSENTER_EIP		equ		06C12h
VMX_VMCS64_GUEST_SYSENTER_EIP 	equ		06826h
   
