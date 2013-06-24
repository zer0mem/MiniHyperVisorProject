pushaq macro
	push r15
	mov r15,rsp
	add r15,sizeof(qword)			;LOAD RSP
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rdi
	push rsi
	push rbp
	push r15							;PUSH RSP
	mov r15,[r15-sizeof(qword)]
	push rbx
	push rdx
	push rcx
	push rax
endm

popaq macro
	pop rax
	pop rcx
	pop rdx
	pop rbx
	add rsp,sizeof(qword)				;SKIP RSP
	pop rbp
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
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

.data

MAGIC							equ		04C495052h
IA32_SYSENTER_EIP				equ		0176h
IA64_SYSENTER_EIP				equ		0C0000082h
Ring3RSP						equ		010h
Ring0RSP						equ		01a8h
VMX_VMCS_HOST_SYSENTER_EIP		equ		06C12h
VMX_VMCS64_GUEST_SYSENTER_EIP 	equ		06826h
