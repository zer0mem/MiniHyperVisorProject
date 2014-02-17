/**
 * @file VmmAutoExit.hpp
 * @author created by: Peter Hlavaty
 */

#ifndef __VMMAUTOEXIT_H__
#define __VMMAUTOEXIT_H__

#include "../Common/base/HVCommon.h"

class CVMMAutoExit
{
#define INTERRUPTION_INFO_TYPE_SHIFT 8
#define INTERRUPTION_INFO_TYPE(a) (((a) >> INTERRUPTION_INFO_TYPE_SHIFT) & 7)
public:
	__forceinline
	CVMMAutoExit() : 
		m_ip(NULL),
		m_insLen(0),
		m_sp(NULL),
		m_flags(0),
		m_intInfo(0)
	{
		if (!vmread(VMX_VMCS32_RO_EXIT_INTERRUPTION_INFO, &m_intInfo))
			if (!vmread(VMX_VMCS64_GUEST_RIP, &m_ip))
				if (!vmread(VMX_VMCS64_GUEST_RSP, &m_sp))
					if (!vmread(VMX_VMCS_GUEST_RFLAGS, &m_flags))
						if (!vmread(VMX_VMCS32_RO_EXIT_REASON, &m_reason))
							vmread(VMX_VMCS32_RO_EXIT_INSTR_LENGTH, &m_insLen);
	}

	__forceinline
	~CVMMAutoExit()
	{
		vmwrite(VMX_VMCS64_GUEST_RIP, m_ip);
		vmwrite(VMX_VMCS_GUEST_RFLAGS, m_flags);
		vmwrite(VMX_VMCS64_GUEST_RSP, m_sp);
	}

	__forceinline
	void DisableTrap()
	{
		m_flags &= ~TRAP;
	}

	__forceinline
	bool IsTrapActive()
	{
		return !!(m_flags & TRAP);
	}

	__forceinline
	const void* GetIp()
	{
		return m_ip;
	}

	__forceinline
	void SetIp(
		__in const void* ip
		)
	{
		m_ip = ip;
	}

	__forceinline
	void SetIpFromCallback(
		__in const void* ip
		)
	{
		m_ip = static_cast<const BYTE*>(ip) - m_insLen;
	}

	__forceinline
	ULONG_PTR* GetSp()
	{
		return m_sp;
	}

	__forceinline
	void SetSp(
		__in ULONG_PTR* sp
		)
	{
		m_sp = sp;
	}

	__forceinline
	ULONG_PTR GetFlags()
	{
		return m_flags;
	}

	__forceinline
	size_t GetInsLen()
	{
		return m_insLen;
	}

	__forceinline
	BYTE GetInterruptionInfo()
	{
		return static_cast<BYTE>(m_intInfo);
	}

	__forceinline
	ULONG_PTR GetReason()
	{
		return m_reason;
	}

protected:
	const void* m_ip;
	size_t m_insLen;
	ULONG_PTR* m_sp;
	ULONG_PTR m_flags;
	ULONG_PTR m_intInfo;
	ULONG_PTR m_reason;
};

#endif //__VMMAUTOEXIT_H__
