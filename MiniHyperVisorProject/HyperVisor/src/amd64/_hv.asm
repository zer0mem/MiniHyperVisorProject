extrn HVExitTrampoline:proc

include ..\Common\amd64\common.inc

.code

@dummy_start:

__hv_rdmsr proc
	rdmsr
	jmp _resume
__hv_rdmsr endp

__hv_wrmsr proc
	wrmsr
	jmp _resume
__hv_wrmsr endp

__hv_cpuid proc	
	cpuid
	jmp _resume
__hv_cpuid endp

__hv_invd proc
	invd
	jmp _resume
__hv_invd endp

__hv_dummy proc
	jmp _resume
__hv_dummy endp

__hv_vmcall proc
	jmp _resume
__hv_vmcall endp

@dummy_end:

hv_exit proc
	pushptr
	pushfq
	pushaq
	
	mov rcx, rsp
	push rcx  ; save &reg ptr ;)
	pushptr  ; space for mov [arg], rcx 
	call HVExitTrampoline
	popptr

;check handler
	lea rcx, @dummy_end
	cmp rax, rcx
	ja _user_specified_callback
	lea rcx, @dummy_start
	cmp rax, rcx
	jb _user_specified_callback

_dummy_callback:
	pop rcx
	mov [rsp + 011h * sizeof(qword)], rax ;ret -> dummy_callback handler
	popaq
	popfq
	ret  ; popptr + jmp to dummy_callback handler

_user_specified_callback:
	mov rcx, [rsp]  ; space for mov [arg], rcx .. not pop.. one instruction save the perfomance world :P
	call rax
	popptr

	popaq  ; cause pop all (valatile && non..) regs, updated from callbacks (&reg)
	popfq
	popptr

	jmp _resume
hv_exit endp
		
_resume:
	vmresume
	ret

end                          
