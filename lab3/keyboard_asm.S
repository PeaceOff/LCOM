
#include "i8042.h"
#.intel_syntax
.global var_global
.global _kbd_test_scan_asm

.extern _printf

.data

	makecode_str:
		.string "MakeCode: 0x%X\n"
	breakcode_str:
		.string "BreakCode: 0x%X\n"

	two_byte_scan:
		.int 0

.text

_kbd_test_scan_asm:

	xor %eax, %eax
	in $KBC_BUFF_ADRESS, %al
	cmp $SCAN_TWO_BYTES,%al
	jne oneByte
	movb $1, two_byte_scan
	jmp end
oneByte:
	cmp $1,two_byte_scan
	jne testCode
	mov $SCAN_TWO_BYTES,%ah
	movb $0,two_byte_scan
testCode:
	xor %ecx, %ecx
	mov %eax, %ecx
	and $BREAK_CODE, %ecx
	cmp $BREAK_CODE, %ecx
	jne makeCode
	xor %ebx,%ebx
	mov %eax,%ebx
	push %eax
	push $breakcode_str
	call _printf
	add $8 ,%esp
	cmpb $ESC_BREAK_CODE, %bl
	je end2
	jmp end
makeCode:
	push %eax
	push $makecode_str
	call _printf
	add $8 ,%esp
end:	mov $0, %eax
	jmp end3
end2:   mov $81, %eax

end3: 	ret







