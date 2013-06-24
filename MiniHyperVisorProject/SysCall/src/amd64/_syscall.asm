extrn SysCallCallback:proc
extrn RdmsrHook:proc

include ..\Common\amd64\common.inc

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