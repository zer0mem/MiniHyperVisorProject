//http://virtualbox-ose.sourcearchive.com/documentation/3.0.10-dfsg-1/group__grp__vmx_g4b5627423dbd6f41ff320a95c7754f47.html

//VMX_EXIT_QUALIFICATION_APIC_ACCESS_TYPE; access types
#define 	VMX_APIC_ACCESS_TYPE_LINEAR_EVENT_DELIVERY   3
#define 	VMX_APIC_ACCESS_TYPE_LINEAR_INSTR_FETCH   2
#define 	VMX_APIC_ACCESS_TYPE_LINEAR_READ   0
#define 	VMX_APIC_ACCESS_TYPE_LINEAR_WRITE   1
#define 	VMX_APIC_ACCESS_TYPE_PHYSICAL_EVENT_DELIVERY   10
#define 	VMX_APIC_ACCESS_TYPE_PHYSICAL_INSTR   15
//VMX_VMCS_GUEST_ACTIVITY_STATE
#define 	VMX_CMS_GUEST_ACTIVITY_ACTIVE   0x0
#define 	VMX_CMS_GUEST_ACTIVITY_HLT   0x1
#define 	VMX_CMS_GUEST_ACTIVITY_SHUTDOWN   0x2
#define 	VMX_CMS_GUEST_ACTIVITY_SIPI_WAIT   0x3
//VMX Basic Exit Reasons.
#define 	VMX_EFLAGS_RESERVED_0   (~0xffc08028)
#define 	VMX_EFLAGS_RESERVED_1   0x00000002
//Extended Page Table Pointer (EPTP)
#define 	VMX_EPT_MEMTYPE_UC   0
#define 	VMX_EPT_MEMTYPE_WB   6
#define 	VMX_EPT_PAGE_WALK_LENGTH_DEFAULT   3
#define 	VMX_EPT_PAGE_WALK_LENGTH_MASK   7
#define 	VMX_EPT_PAGE_WALK_LENGTH_SHIFT   3
//VM Instruction Errors
#define 	VMX_ERROR_INVEPTVPID_INVALID_OPERAND   28
#define 	VMX_ERROR_VMCALL   1
#define 	VMX_ERROR_VMCALL_INVALID_MSEG_REVISION   22
#define 	VMX_ERROR_VMCALL_INVALID_SMM_MONITOR   24
#define 	VMX_ERROR_VMCALL_INVALID_VMEXIT_FIELDS   20
#define 	VMX_ERROR_VMCALL_NON_CLEAR_VMCS   19
#define 	VMX_ERROR_VMCLEAR_INVALID_PHYS_ADDR   2
#define 	VMX_ERROR_VMCLEAR_INVALID_VMXON_PTR   3
#define 	VMX_ERROR_VMENTRY_EXEC_VMCS_PTR   18
#define 	VMX_ERROR_VMENTRY_INVALID_CONTROL_FIELDS   7
#define 	VMX_ERROR_VMENTRY_INVALID_HOST_STATE   8
#define 	VMX_ERROR_VMENTRY_INVALID_VM_EXEC_CTRL   25
#define 	VMX_ERROR_VMENTRY_INVALID_VMCS_EXEC_PTR   16
#define 	VMX_ERROR_VMENTRY_MOV_SS   26
#define 	VMX_ERROR_VMENTRY_NON_LAUNCHED_EXEC_VMCS   17
#define 	VMX_ERROR_VMLAUCH_NON_CLEAR_VMCS   4
#define 	VMX_ERROR_VMPTRLD_INVALID_PHYS_ADDR   9
#define 	VMX_ERROR_VMPTRLD_VMXON_PTR   10
#define 	VMX_ERROR_VMPTRLD_WRONG_VMCS_REVISION   11
#define 	VMX_ERROR_VMREAD_INVALID_COMPONENT   12
#define 	VMX_ERROR_VMRESUME_CORRUPTED_VMCS   6
#define 	VMX_ERROR_VMRESUME_NON_LAUNCHED_VMCS   5
#define 	VMX_ERROR_VMWRITE_INVALID_COMPONENT   VMX_ERROR_VMREAD_INVALID_COMPONENT
#define 	VMX_ERROR_VMWRITE_READONLY_COMPONENT   13
#define 	VMX_ERROR_VMXOFF_DUAL_MONITOR   23
#define 	VMX_ERROR_VMXON_IN_VMX_ROOT_OP   15
//VMX Basic Exit Reasons.
#define 	VMX_EXIT_APIC_ACCESS   44
#define 	VMX_EXIT_CPUID   10
#define 	VMX_EXIT_CRX_MOVE   28
#define 	VMX_EXIT_DRX_MOVE   29
#define 	VMX_EXIT_EPT_MISCONFIG   49
#define 	VMX_EXIT_EPT_VIOLATION   48
#define 	VMX_EXIT_ERR_INVALID_GUEST_STATE   33
#define 	VMX_EXIT_ERR_MACHINE_CHECK   41
#define 	VMX_EXIT_ERR_MSR_LOAD   34
#define 	VMX_EXIT_EXCEPTION   0
#define 	VMX_EXIT_EXTERNAL_IRQ   1
#define 	VMX_EXIT_HLT   12
#define 	VMX_EXIT_INIT_SIGNAL   3
#define 	VMX_EXIT_INVALID   -1
#define 	VMX_EXIT_INVD   13
#define 	VMX_EXIT_INVEPT   50
#define 	VMX_EXIT_INVPG   14
#define 	VMX_EXIT_INVVPID   53
#define 	VMX_EXIT_IO_SMI_IRQ   5
#define 	VMX_EXIT_IRQ_WINDOW   7
#define 	VMX_EXIT_MONITOR   39
#define 	VMX_EXIT_MWAIT   36
#define 	VMX_EXIT_PAUSE   40
#define 	VMX_EXIT_PORT_IO   30
#define 	VMX_EXIT_PREEMPTION_TIMER   52
#define 	VMX_EXIT_RDMSR   31
#define 	VMX_EXIT_RDPMC   15
#define 	VMX_EXIT_RDTSC   16
#define 	VMX_EXIT_RSM   17
#define 	VMX_EXIT_SIPI   4
#define 	VMX_EXIT_SMI_IRQ   6
#define 	VMX_EXIT_TASK_SWITCH   9
#define 	VMX_EXIT_TPR   43
#define 	VMX_EXIT_TR_ACCESS   47
#define 	VMX_EXIT_TRIPLE_FAULT   2
#define 	VMX_EXIT_VMCALL   18
#define 	VMX_EXIT_VMCLEAR   19
#define 	VMX_EXIT_VMLAUNCH   20
#define 	VMX_EXIT_VMPTRLD   21
#define 	VMX_EXIT_VMPTRST   22
#define 	VMX_EXIT_VMREAD   23
#define 	VMX_EXIT_VMRESUME   24
#define 	VMX_EXIT_VMWRITE   25
#define 	VMX_EXIT_VMXOFF   26
#define 	VMX_EXIT_VMXON   27
#define 	VMX_EXIT_WBINVD   54
#define 	VMX_EXIT_WRMSR   32
#define 	VMX_EXIT_XDTR_ACCESS   46
#define 	VMX_EXIT_XSETBV   55

//VMCS field encoding - 16 bits guest fields
#define 	VMX_VMCS16_GUEST_FIELD_CS   0x802
#define 	VMX_VMCS16_GUEST_FIELD_DS   0x806
#define 	VMX_VMCS16_GUEST_FIELD_ES   0x800
#define 	VMX_VMCS16_GUEST_FIELD_FS   0x808
#define 	VMX_VMCS16_GUEST_FIELD_GS   0x80A
#define 	VMX_VMCS16_GUEST_FIELD_LDTR   0x80C
#define 	VMX_VMCS16_GUEST_FIELD_SS   0x804
#define 	VMX_VMCS16_GUEST_FIELD_TR   0x80E
#define 	VMX_VMCS16_GUEST_FIELD_VPID   0x0
//VMCS field encoding - 16 bits host fields
#define 	VMX_VMCS16_HOST_FIELD_CS   0xC02
#define 	VMX_VMCS16_HOST_FIELD_DS   0xC06
#define 	VMX_VMCS16_HOST_FIELD_ES   0xC00
#define 	VMX_VMCS16_HOST_FIELD_FS   0xC08
#define 	VMX_VMCS16_HOST_FIELD_GS   0xC0A
#define 	VMX_VMCS16_HOST_FIELD_SS   0xC04
#define 	VMX_VMCS16_HOST_FIELD_TR   0xC0C
//VMCS field encoding - 32 Bits guest state fields
#define 	VMX_VMCS32_GUEST_ACTIVITY_STATE   0x4826
#define 	VMX_VMCS32_GUEST_CS_ACCESS_RIGHTS   0x4816
#define 	VMX_VMCS32_GUEST_CS_LIMIT   0x4802
#define 	VMX_VMCS32_GUEST_DS_ACCESS_RIGHTS   0x481A
#define 	VMX_VMCS32_GUEST_DS_LIMIT   0x4806
#define 	VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS   0x4814
#define 	VMX_VMCS32_GUEST_ES_LIMIT   0x4800
#define 	VMX_VMCS32_GUEST_FS_ACCESS_RIGHTS   0x481C
#define 	VMX_VMCS32_GUEST_FS_LIMIT   0x4808
#define 	VMX_VMCS32_GUEST_GDTR_LIMIT   0x4810
#define 	VMX_VMCS32_GUEST_GS_ACCESS_RIGHTS   0x481E
#define 	VMX_VMCS32_GUEST_GS_LIMIT   0x480A
#define 	VMX_VMCS32_GUEST_IDTR_LIMIT   0x4812
#define 	VMX_VMCS32_GUEST_INTERRUPTIBILITY_STATE   0x4824
#define 	VMX_VMCS32_GUEST_LDTR_ACCESS_RIGHTS   0x4820
#define 	VMX_VMCS32_GUEST_LDTR_LIMIT   0x480C
#define 	VMX_VMCS32_GUEST_PREEMPTION_TIMER_VALUE   0x482E
#define 	VMX_VMCS32_GUEST_SS_ACCESS_RIGHTS   0x4818
#define 	VMX_VMCS32_GUEST_SS_LIMIT   0x4804
#define 	VMX_VMCS32_GUEST_SYSENTER_CS   0x482A
#define 	VMX_VMCS32_GUEST_TR_ACCESS_RIGHTS   0x4822
#define 	VMX_VMCS32_GUEST_TR_LIMIT   0x480E
//VMCS field encoding - 32 Bits host state fields
#define 	VMX_VMCS32_HOST_SYSENTER_CS   0x4C00
//VMCS field encoding - 32 Bits read-only fields
#define 	VMX_VMCS32_RO_EXIT_INSTR_INFO   0x440E
#define 	VMX_VMCS32_RO_EXIT_INSTR_LENGTH   0x440C
#define 	VMX_VMCS32_RO_EXIT_INTERRUPTION_ERRCODE   0x4406
#define 	VMX_VMCS32_RO_EXIT_INTERRUPTION_INFO   0x4404
#define 	VMX_VMCS32_RO_EXIT_REASON   0x4402
#define 	VMX_VMCS32_RO_IDT_ERRCODE   0x440A
#define 	VMX_VMCS32_RO_IDT_INFO   0x4408
#define 	VMX_VMCS32_RO_VM_INSTR_ERROR   0x4400
//VMCS field encoding - Natural width guest state fields
#define 	VMX_VMCS64_GUEST_CR0   0x6800
#define 	VMX_VMCS64_GUEST_CR3   0x6802
#define 	VMX_VMCS64_GUEST_CR4   0x6804
#define 	VMX_VMCS64_GUEST_CS_BASE   0x6808
#define 	VMX_VMCS64_GUEST_DR7   0x681A
#define 	VMX_VMCS64_GUEST_DS_BASE   0x680C
#define 	VMX_VMCS64_GUEST_ES_BASE   0x6806
#define 	VMX_VMCS64_GUEST_FS_BASE   0x680E
#define 	VMX_VMCS64_GUEST_GDTR_BASE   0x6816
#define 	VMX_VMCS64_GUEST_GS_BASE   0x6810
#define 	VMX_VMCS64_GUEST_IDTR_BASE   0x6818
#define 	VMX_VMCS64_GUEST_LDTR_BASE   0x6812
#define 	VMX_VMCS64_GUEST_RIP   0x681E
#define 	VMX_VMCS64_GUEST_RSP   0x681C
#define 	VMX_VMCS64_GUEST_SS_BASE   0x680A
#define 	VMX_VMCS64_GUEST_SYSENTER_EIP   0x6826
#define 	VMX_VMCS64_GUEST_SYSENTER_ESP   0x6824
#define 	VMX_VMCS64_GUEST_TR_BASE   0x6814
#define 	VMX_VMCS_GUEST_DEBUG_EXCEPTIONS   0x6822
#define 	VMX_VMCS_GUEST_RFLAGS   0x6820
//VMCS field encoding - 64 Bits control fields
#define 	VMX_VMCS_CTRL_APIC_ACCESSADDR_FULL   0x2014
#define 	VMX_VMCS_CTRL_APIC_ACCESSADDR_HIGH   0x2015
#define 	VMX_VMCS_CTRL_EPTP_FULL   0x201a
#define 	VMX_VMCS_CTRL_EPTP_HIGH   0x201b
#define 	VMX_VMCS_CTRL_EXEC_VMCS_PTR_FULL   0x200C
#define 	VMX_VMCS_CTRL_EXEC_VMCS_PTR_HIGH   0x200D
#define 	VMX_VMCS_CTRL_IO_BITMAP_A_FULL   0x2000
#define 	VMX_VMCS_CTRL_IO_BITMAP_A_HIGH   0x2001
#define 	VMX_VMCS_CTRL_IO_BITMAP_B_FULL   0x2002
#define 	VMX_VMCS_CTRL_IO_BITMAP_B_HIGH   0x2003
#define 	VMX_VMCS_CTRL_MSR_BITMAP_FULL   0x2004
#define 	VMX_VMCS_CTRL_MSR_BITMAP_HIGH   0x2005
#define 	VMX_VMCS_CTRL_TSC_OFFSET_FULL   0x2010
#define 	VMX_VMCS_CTRL_TSC_OFFSET_HIGH   0x2011
#define 	VMX_VMCS_CTRL_VAPIC_PAGEADDR_FULL   0x2012
#define 	VMX_VMCS_CTRL_VAPIC_PAGEADDR_HIGH   0x2013
#define 	VMX_VMCS_CTRL_VMENTRY_MSR_LOAD_FULL   0x200A
#define 	VMX_VMCS_CTRL_VMENTRY_MSR_LOAD_HIGH   0x200B
#define 	VMX_VMCS_CTRL_VMEXIT_MSR_LOAD_FULL   0x2008
#define 	VMX_VMCS_CTRL_VMEXIT_MSR_LOAD_HIGH   0x2009
#define 	VMX_VMCS_CTRL_VMEXIT_MSR_STORE_FULL   0x2006
#define 	VMX_VMCS_CTRL_VMEXIT_MSR_STORE_HIGH   0x2007
#define 	VMX_VMCS_EXIT_PHYS_ADDR_FULL   0x2400
#define 	VMX_VMCS_EXIT_PHYS_ADDR_HIGH   0x2401
//Natural width control fields
#define 	VMX_VMCS_CTRL_CR0_MASK   0x6000
#define 	VMX_VMCS_CTRL_CR0_READ_SHADOW   0x6004
#define 	VMX_VMCS_CTRL_CR3_TARGET_VAL0   0x6008
#define 	VMX_VMCS_CTRL_CR3_TARGET_VAL1   0x600A
#define 	VMX_VMCS_CTRL_CR3_TARGET_VAL2   0x600C
#define 	VMX_VMCS_CTRL_CR3_TARGET_VAL31   0x600E
#define 	VMX_VMCS_CTRL_CR4_MASK   0x6002
#define 	VMX_VMCS_CTRL_CR4_READ_SHADOW   0x6006
//VMCS field encoding - 32 Bits control fields
#define 	VMX_VMCS_CTRL_CR3_TARGET_COUNT   0x400A
#define 	VMX_VMCS_CTRL_ENTRY_CONTROLS   0x4012
#define 	VMX_VMCS_CTRL_ENTRY_EXCEPTION_ERRCODE   0x4018
#define 	VMX_VMCS_CTRL_ENTRY_INSTR_LENGTH   0x401A
#define 	VMX_VMCS_CTRL_ENTRY_IRQ_INFO   0x4016
#define 	VMX_VMCS_CTRL_ENTRY_MSR_LOAD_COUNT   0x4014
#define 	VMX_VMCS_CTRL_EXCEPTION_BITMAP   0x4004
#define 	VMX_VMCS_CTRL_EXIT_CONTROLS   0x400C
#define 	VMX_VMCS_CTRL_EXIT_MSR_LOAD_COUNT   0x4010
#define 	VMX_VMCS_CTRL_EXIT_MSR_STORE_COUNT   0x400E
#define 	VMX_VMCS_CTRL_PAGEFAULT_ERROR_MASK   0x4006
#define 	VMX_VMCS_CTRL_PAGEFAULT_ERROR_MATCH   0x4008
#define 	VMX_VMCS_CTRL_PIN_EXEC_CONTROLS   0x4000
#define 	VMX_VMCS_CTRL_PROC_EXEC_CONTROLS   0x4002
#define 	VMX_VMCS_CTRL_PROC_EXEC_CONTROLS2   0x401E
#define 	VMX_VMCS_CTRL_TPR_THRESHOLD   0x401C

//Natural width read-only data fields
#define 	VMX_VMCS_EXIT_GUEST_LINEAR_ADDR   0x640A
#define 	VMX_VMCS_RO_EXIT_QUALIFICATION   0x6400
#define 	VMX_VMCS_RO_IO_RCX   0x6402
#define 	VMX_VMCS_RO_IO_RDI   0x6406
#define 	VMX_VMCS_RO_IO_RIP   0x6408
#define 	VMX_VMCS_RO_IO_RSX   0x6404

//VMCS field encoding - 64 Bits guest fields
#define 	VMX_VMCS_GUEST_DEBUGCTL_FULL   0x2802
#define 	VMX_VMCS_GUEST_DEBUGCTL_HIGH   0x2803
#define 	VMX_VMCS_GUEST_EFER_FULL   0x2806
#define 	VMX_VMCS_GUEST_EFER_HIGH   0x2807
#define 	VMX_VMCS_GUEST_LINK_PTR_FULL   0x2800
#define 	VMX_VMCS_GUEST_LINK_PTR_HIGH   0x2801
#define 	VMX_VMCS_GUEST_PAT_FULL   0x2804
#define 	VMX_VMCS_GUEST_PAT_HIGH   0x2805
#define 	VMX_VMCS_GUEST_PDPTR0_FULL   0x280A
#define 	VMX_VMCS_GUEST_PDPTR0_HIGH   0x280B
#define 	VMX_VMCS_GUEST_PDPTR1_FULL   0x280C
#define 	VMX_VMCS_GUEST_PDPTR1_HIGH   0x280D
#define 	VMX_VMCS_GUEST_PDPTR2_FULL   0x280E
#define 	VMX_VMCS_GUEST_PDPTR2_HIGH   0x280F
#define 	VMX_VMCS_GUEST_PDPTR3_FULL   0x2810
#define 	VMX_VMCS_GUEST_PDPTR3_HIGH   0x2811
#define 	VMX_VMCS_GUEST_PERF_GLOBAL_CTRL_FULL   0x2808
#define 	VMX_VMCS_GUEST_PERF_GLOBAL_CTRL_HIGH   0x2809


#define 	VMX_VMCS_HOST_CR0   0x6C00
#define 	VMX_VMCS_HOST_CR3   0x6C02
#define 	VMX_VMCS_HOST_CR4   0x6C04
#define 	VMX_VMCS_HOST_FS_BASE   0x6C06
#define 	VMX_VMCS_HOST_GDTR_BASE   0x6C0C
#define 	VMX_VMCS_HOST_GS_BASE   0x6C08
#define 	VMX_VMCS_HOST_IDTR_BASE   0x6C0E
#define 	VMX_VMCS_HOST_RIP   0x6C16
#define 	VMX_VMCS_HOST_RSP   0x6C14
#define 	VMX_VMCS_HOST_SYSENTER_EIP   0x6C12
#define 	VMX_VMCS_HOST_SYSENTER_ESP   0x6C10
#define 	VMX_VMCS_HOST_TR_BASE   0x6C0A

//VMCS field encoding - 64 bits host fields
#define 	VMX_VMCS_HOST_FIELD_EFER_FULL   0x2C02
#define 	VMX_VMCS_HOST_FIELD_EFER_HIGH   0x2C03
#define 	VMX_VMCS_HOST_FIELD_PAT_FULL   0x2C00
#define 	VMX_VMCS_HOST_FIELD_PAT_HIGH   0x2C01
#define 	VMX_VMCS_HOST_PERF_GLOBAL_CTRL_FULL   0x2C04
#define 	VMX_VMCS_HOST_PERF_GLOBAL_CTRL_HIGH   0x2C05

//x64
#define		VMX_VMCS32_EXIT_IA32E_MODE			0x200
#define		VMX_VMCS32_EXIT_ACK_ITR_ON_EXIT		0x8000
#define		VMX_VMCS32_ENTRY_IA32E_MODE			0x200



//processor.h -> cpu.h
//http://xenbits.xen.org/git-http/people/jm77ryu/xen-unstable-arm.hg/xen/include/asm-x86/processor.h

#define X86_VENDOR_INTEL 0
#define X86_VENDOR_CYRIX 1
#define X86_VENDOR_AMD 2
#define X86_VENDOR_UMC 3
#define X86_VENDOR_NEXGEN 4
#define X86_VENDOR_CENTAUR 5
#define X86_VENDOR_RISE 6
#define X86_VENDOR_TRANSMETA 7
#define X86_VENDOR_NSC 8
#define X86_VENDOR_NUM 9
#define X86_VENDOR_UNKNOWN 0xff

/*
 * EFLAGS bits
 */
#define X86_EFLAGS_CF	0x00000001 /* Carry Flag */
#define X86_EFLAGS_PF	0x00000004 /* Parity Flag */
#define X86_EFLAGS_AF	0x00000010 /* Auxillary carry Flag */
#define X86_EFLAGS_ZF	0x00000040 /* Zero Flag */
#define X86_EFLAGS_SF	0x00000080 /* Sign Flag */
#define X86_EFLAGS_TF	0x00000100 /* Trap Flag */
#define X86_EFLAGS_IF	0x00000200 /* Interrupt Flag */
#define X86_EFLAGS_DF	0x00000400 /* Direction Flag */
#define X86_EFLAGS_OF	0x00000800 /* Overflow Flag */
#define X86_EFLAGS_IOPL	0x00003000 /* IOPL mask */
#define X86_EFLAGS_NT	0x00004000 /* Nested Task */
#define X86_EFLAGS_RF	0x00010000 /* Resume Flag */
#define X86_EFLAGS_VM	0x00020000 /* Virtual Mode */
#define X86_EFLAGS_AC	0x00040000 /* Alignment Check */
#define X86_EFLAGS_VIF	0x00080000 /* Virtual Interrupt Flag */
#define X86_EFLAGS_VIP	0x00100000 /* Virtual Interrupt Pending */
#define X86_EFLAGS_ID	0x00200000 /* CPUID detection flag */

/*
 * Intel CPU flags in CR0
 */
#define X86_CR0_PE              0x00000001 /* Enable Protected Mode    (RW) */
#define X86_CR0_MP              0x00000002 /* Monitor Coprocessor      (RW) */
#define X86_CR0_EM              0x00000004 /* Require FPU Emulation    (RO) */
#define X86_CR0_TS              0x00000008 /* Task Switched            (RW) */
#define X86_CR0_ET              0x00000010 /* Extension type           (RO) */
#define X86_CR0_NE              0x00000020 /* Numeric Error Reporting  (RW) */
#define X86_CR0_WP              0x00010000 /* Supervisor Write Protect (RW) */
#define X86_CR0_AM              0x00040000 /* Alignment Checking       (RW) */
#define X86_CR0_NW              0x20000000 /* Not Write-Through        (RW) */
#define X86_CR0_CD              0x40000000 /* Cache Disable            (RW) */
#define X86_CR0_PG              0x80000000 /* Paging                   (RW) */

/*
 * Intel CPU features in CR4
 */
#define X86_CR4_VME		0x0001	/* enable vm86 extensions */
#define X86_CR4_PVI		0x0002	/* virtual interrupts flag enable */
#define X86_CR4_TSD		0x0004	/* disable time stamp at ipl 3 */
#define X86_CR4_DE		0x0008	/* enable debugging extensions */
#define X86_CR4_PSE		0x0010	/* enable page size extensions */
#define X86_CR4_PAE		0x0020	/* enable physical address extensions */
#define X86_CR4_MCE		0x0040	/* Machine check enable */
#define X86_CR4_PGE		0x0080	/* enable global pages */
#define X86_CR4_PCE		0x0100	/* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR		0x0200	/* enable fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT	0x0400	/* enable unmasked SSE exceptions */
#define X86_CR4_VMXE		0x2000  /* enable VMX */
#define X86_CR4_SMXE		0x4000  /* enable SMX */
#define X86_CR4_FSGSBASE	0x10000 /* enable {rd,wr}{fs,gs}base */
#define X86_CR4_OSXSAVE	0x40000 /* enable XSAVE/XRSTOR */
#define X86_CR4_SMEP		0x100000/* enable SMEP */

/*
 * Trap/fault mnemonics.
 */
#define TRAP_divide_error      0
#define TRAP_debug             1
#define TRAP_nmi               2
#define TRAP_int3              3
#define TRAP_overflow          4
#define TRAP_bounds            5
#define TRAP_invalid_op        6
#define TRAP_no_device         7
#define TRAP_double_fault      8
#define TRAP_copro_seg         9
#define TRAP_invalid_tss      10
#define TRAP_no_segment       11
#define TRAP_stack_error      12
#define TRAP_gp_fault         13
#define TRAP_page_fault       14
#define TRAP_spurious_int     15
#define TRAP_copro_error      16
#define TRAP_alignment_check  17
#define TRAP_machine_check    18
#define TRAP_simd_error       19

/* Set for entry via SYSCALL. Informs return code to use SYSRETQ not IRETQ. */
/* NB. Same as VGCF_in_syscall. No bits in common with any other TRAP_ defn. */
#define TRAP_syscall         256

/* Boolean return code: the reason for a fault has been fixed. */
#define EXCRET_fault_fixed 1

/* 'trap_bounce' flags values */
#define TBF_EXCEPTION          1
#define TBF_EXCEPTION_ERRCODE  2
#define TBF_INTERRUPT          8
#define TBF_FAILSAFE          16

/* 'arch_vcpu' flags values */
#define _TF_kernel_mode        0
#define TF_kernel_mode         (1<<_TF_kernel_mode)

/* #PF error code values. */
#define PFEC_page_present   (1U<<0)
#define PFEC_write_access   (1U<<1)
#define PFEC_user_mode      (1U<<2)
#define PFEC_reserved_bit   (1U<<3)
#define PFEC_insn_fetch     (1U<<4)
#define PFEC_page_paged     (1U<<5)
#define PFEC_page_shared    (1U<<6)


#define DR7_ENABLED_MASK 0xFF
#define DR7_GD (1 << 13)

/*bluepill*/
#define CPU_BASED_VIRTUAL_INTR_PENDING  0x00000004
#define CPU_BASED_USE_TSC_OFFSETING     0x00000008
#define CPU_BASED_HLT_EXITING           0x00000080
#define CPU_BASED_INVDPG_EXITING        0x00000200
#define CPU_BASED_MWAIT_EXITING         0x00000400
#define CPU_BASED_RDPMC_EXITING         0x00000800
#define CPU_BASED_RDTSC_EXITING         0x00001000
#define CPU_BASED_CR8_LOAD_EXITING      0x00080000
#define CPU_BASED_CR8_STORE_EXITING     0x00100000
#define CPU_BASED_TPR_SHADOW            0x00200000
#define CPU_BASED_MOV_DR_EXITING        0x00800000
#define CPU_BASED_UNCOND_IO_EXITING     0x01000000
#define CPU_BASED_ACTIVATE_IO_BITMAP    0x02000000
#define CPU_BASED_ACTIVATE_MSR_BITMAP   0x10000000
#define CPU_BASED_MONITOR_EXITING       0x20000000
#define CPU_BASED_PAUSE_EXITING         0x40000000

#define PIN_BASED_EXT_INTR_MASK         0x00000001
#define PIN_BASED_NMI_EXITING           0x00000008