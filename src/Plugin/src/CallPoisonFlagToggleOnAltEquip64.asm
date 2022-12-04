EXTERNDEF TogglePoisonFlagIfAlternateEquipped : PROC

.DATA?
	RetAddr QWORD ?
	Equipper QWORD ?
	EquippingItem QWORD ?
	EquippingSlot QWORD ?

.CODE
CallPoisonFlagToggleOnAltEquip PROC

	pop RetAddr ; Save the return address from the top of the stack

	; Grab the arguments being passed into the EquipItem function we are intercepting
	mov Equipper, rdx
	mov rax, qword ptr [r8]
	mov EquippingItem, rax
	mov EquippingSlot, r9

	; Preserve registers and the stack
	pushf
	push rax
	push rcx
	push rdx
	push rbx
	push rsp
	push rbp
	push rsi
	push rdi
	push r8
	push r9

	; Per the Microsoft x64 calling convention, function callers are always responsible for allocating 32 bytes of shadow space
	sub rsp, 20h

	; Call function
	mov rcx, Equipper
	mov rdx, EquippingItem
	mov r8, EquippingSlot
	call TogglePoisonFlagIfAlternateEquipped

	; Free the shadow space allocated for the function call
	add rsp, 20h

	; Restore everything to how it was before
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rsp
	pop rbx
	pop rdx
	pop rcx
	pop rax
	popf

	pop rax ; The codecave call used RAX to point to this function's address - we need to restore the value that RAX held before that

Overwritten:
	mov qword ptr [RSP + 8h], rbx
	mov qword ptr [RSP + 18h], r8
	push rbp
	push rsi
	push rdi

CodecaveEnd:
	push RetAddr
	ret

CallPoisonFlagToggleOnAltEquip ENDP

END