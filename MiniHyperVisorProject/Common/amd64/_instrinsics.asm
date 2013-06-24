.code

__ror proc
	mov rax, rcx
	mov rcx, rdx
	ror rax, cl
	ret
__ror endp

__rol proc
	mov rax, rcx
	mov rcx, rdx
	rol rax, cl
	ret
__rol endp

__sgdt proc
	sgdt fword ptr[rcx]
	ret
__sgdt endp

__str proc
	str rax
	ret
__str endp

__sldt proc
	sldt rax
	ret
__sldt endp

__vmx_call proc
	vmcall
	ret
__vmx_call endp

__readcs proc
	mov rax, cs
	ret
__readcs endp

__readds proc
	mov rax, ds
	ret
__readds endp

__reades proc
	mov rax, es
	ret
__reades endp

__readss proc
	mov rax, ss
	ret
__readss endp

__readfs proc
	mov rax, fs
	ret
__readfs endp

__readgs proc
	mov rax, gs
	ret
__readgs endp

__writeds proc
	mov ds, rcx
	ret
__writeds endp

__xchgds proc
	mov rax, ds
	mov ds, [rcx]
	mov [rcx], rax
	ret
__xchgds endp

__cli proc
	cli
	ret
__cli endp

__sti proc
	cli
	ret
__sti endp

__kebreak proc
	int 3
	ret
__kebreak endp

__x64_cpuid proc
    mov rax,rcx
    cpuid
    ret
__x64_cpuid endp

end