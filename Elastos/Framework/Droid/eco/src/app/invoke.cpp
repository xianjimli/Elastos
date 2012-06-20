
#ifdef __cplusplus
extern "C" {
#endif

__declspec(naked)
int invoke(void* pThis, void* pFunc, void* pParam, int nSize)
{
    __asm {
		push    ebp
	    mov     ebp, esp
        mov     ecx, dword ptr [ebp + 20]  //nSize
        mov     eax, dword ptr [ebp + 16]  //pParam
        add     eax, ecx
        sub     eax, 4
push_param:
        test    ecx, ecx
        jz      do_call

        mov     edx, dword ptr [eax]
        push    edx
        sub     eax, 4
        sub     ecx, 4
        jmp     push_param
do_call:
        mov     edx, dword ptr [ebp + 8]  //pThis
        push    edx
        mov     ecx, dword ptr [ebp + 12]  //pFunc
        call    ecx

        mov     esp, ebp
        pop     ebp
        ret
    }
}

#ifdef __cplusplus
}
#endif
