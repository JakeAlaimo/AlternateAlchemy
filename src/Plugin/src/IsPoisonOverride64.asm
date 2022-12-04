EXTERNDEF OverrideFormID : DWORD
EXTERNDEF OverridePoisonState : BYTE

.DATA?
	RetAddr QWORD ?

.CODE
IsPoisonOverride PROC

	pop RetAddr ; Save the return address from the top of the stack
	pop rax ; The codecave call used RAX to point to this function's address - let's yank that off the stack

CheckOverride: ; Override the IsPoison check IFF the form passed in has been flagged to be overridden
	test rcx, rcx
	jz CheckPoisonFlag
	mov edx, dword ptr [rcx + 14h]
	cmp edx, OverrideFormID
	jnz CheckPoisonFlag
	mov al, OverridePoisonState
	and al, 1h
	mov OverridePoisonState, 0
	mov OverrideFormID, 0
	jmp CodecaveEnd
	
CheckPoisonFlag: ; Overwritten, original IsPosion body
	sub rsp, 28h
	mov rax, qword ptr [rcx]
	call qword ptr [rax + 360h]	
	mov eax, dword ptr [rax + 4h]
	shr eax, 11h
	and al, 1h
	add rsp, 28h

CodecaveEnd:
	push RetAddr
	ret

IsPoisonOverride ENDP

END