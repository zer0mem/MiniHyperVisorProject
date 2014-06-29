.data

include common.inc

extrn SysCallCallback:proc
extrn RdmsrHook:proc

MAGIC							equ		04C495052h
IA32_SYSENTER_EIP				equ		0176h
IA64_SYSENTER_EIP				equ		0C0000082h
Ring3RSP						equ		010h
Ring0RSP						equ		01a8h
VMX_VMCS_HOST_SYSENTER_EIP		equ		06C12h
VMX_VMCS64_GUEST_SYSENTER_EIP 	equ		06826h

.code

sysenter proc
	swapgs	
	mov qword ptr gs:[Ring3RSP],rsp
	mov rsp,qword ptr gs:[Ring0RSP]
_hook:

	push rax
	pushfq
	pushaq

	mov rcx, rsp
	pushptr
	call SysCallCallback
	popptr

	add rax, _hook - sysenter
	xchg [rsp], rax
	popaq
	popfq

	xchg [rsp], rax
	ret
sysenter endp

rdmsr_hook proc
	push rax ; jmp addr {ret}
	pushfq
	pushaq
	
	mov rcx, rsp
	pushptr
	call RdmsrHook
	popptr

	xchg [rsp], rax ; set jmp addr {ret} to rax
	popaq
	popfq
	
	xchg [rsp], rax ; final set ret addr to jmp -> original hooked fnctn
	ret
rdmsr_hook endp

end