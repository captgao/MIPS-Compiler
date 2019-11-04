.data
	.align 2
	__glbl_var_x: .word 0
	.align 2
	__glbl_var_p: .word 0
	.align 2
	__glbl_var_y: .word 0
	.align 2
	__glbl_var_z: .word 0
	.align 2
	__glbl_var_test: .word 0
	.align 2
__glbl_array_apples: .space 400
	.align 2
__glbl_array_str: .space 100
	.align 2
__string_0: .asciiz "empty parameter list & condition is a single expression"
	.align 2
__string_1: .asciiz "as plain as daylight 1\\n"
	.align 2
__string_2: .asciiz "as plain as daylight 2"
	.align 2
__string_3: .asciiz "Print an expression alone & print string with expression & all the <zi fu>"
	.align 2
__string_4: .asciiz "apples[0] is"
	.align 2
__string_5: .asciiz " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
	.align 2
__string_6: .asciiz "Some empty sentences"
	.align 2
__string_7: .asciiz "sreg[count] is"
	.align 2
__string_8: .asciiz "if & switch & condition is a relation expression & all valid string & valid char,num"
	.align 2
__string_9: .asciiz "A valid <zi fu>"
	.align 2
__string_10: .asciiz "char 34 is not included!"
	.align 2
__string_11: .asciiz "Input two numbers to calculate GCD"
	.align 2
__string_12: .asciiz "Two input for GCD "
	.align 2
__string_13: .asciiz "Greatest Common Division of "
	.align 2
__string_14: .asciiz "is "
	.align 2
__string_15: .asciiz "100"
	.align 2
__string_16: .asciiz "Hello Worl"
	.align 2
__string_17: .asciiz "Empty Default Case"
.text
	 j main
BoringGreetings:
	addiu $29,$29,-372
	sw $31,48($29)
	sw $16,16($29)
	sw $17,20($29)
	sw $18,24($29)
	sw $19,28($29)
	sw $20,32($29)
	sw $21,36($29)
	sw $22,40($29)
	sw $23,44($29)
#move
	li $25,2
__label_1:
#calculate
	lw $16,368($29)
	li $25,2
	mul $17,$16,$25
#calculate
	li $24,3
	mul $18,$17,$24
#calculate
	li $25,4
	div $19,$18,$25
#notequal
	beq $19,$0,__label_2
#printstring
	li $2,4
	la $a0,__string_0
	syscall
#print
	li $2,1
	lw $16,368($29)
	move $a0,$16
	syscall
#calculate
	li $24,1
	subu $16,$16,$24
#jmp
	j __label_1
__label_2:
#calculate
	li $24,1
	li $25,1
	addu $17,$24,$25
#lessthan
	li $24,3
	bge $17,$24,__label_3
#printstring
	li $2,4
	la $a0,__string_1
	syscall
__label_3:
#calculate
	li $25,1
	li $24,1
	addu $16,$25,$24
#greaterthan
	li $25,1
	ble $16,$25,__label_4
#printstring
	li $2,4
	la $a0,__string_2
	syscall
__label_4:
#printstring
	li $2,4
	la $a0,__string_3
	syscall
#assigni
	sll $0,$0,2
	li $24,1
	sw $24,__glbl_array_apples($0)
#assigni
	li $25,1
	sll $25,$25,2
	li $24,12340
	sw $24,__glbl_array_apples($25)
#assigni
	li $25,2
	sll $25,$25,2
	li $24,56789
	sw $24,__glbl_array_apples($25)
#calculate
	li $25,97
	addu $16,$25,$0
#equalto
	li $24,97
	bne $24,$16,__label_5
#index
	la $9,__glbl_array_apples
	sll $0,$0,2
	addu $9,$9,$0
	lw $16,0($9)
#printstring
	li $2,4
	la $a0,__string_4
	syscall
#print
	li $2,1
	move $a0,$16
	syscall
__label_5:
#calculate
	li $25,-1
	subu $17,$0,$25
#calculate
	li $24,10
	mul $18,$17,$24
#calculate
	li $25,10
	div $19,$18,$25
#calculate
	li $24,97
	li $25,10
	mul $20,$24,$25
#calculate
	li $24,97
	div $21,$20,$24
#calculate
	addu $22,$19,$21
#index
	la $9,__glbl_array_apples
	sll $0,$0,2
	addu $9,$9,$0
	lw $23,0($9)
#calculate
	sw $16,96($29)
	addu $16,$22,$23
#calculate
	li $25,95
	li $24,43
	sw $17,104($29)
	mul $17,$25,$24
#calculate
	li $25,45
	li $24,42
	sw $18,108($29)
	addu $18,$25,$24
#calculate
	sw $19,112($29)
	div $19,$17,$18
#calculate
	sw $20,116($29)
	addu $20,$16,$19
#calculate
	li $25,47
	sw $21,120($29)
	addu $21,$20,$25
#calculate
	li $24,65
	sw $22,124($29)
	addu $22,$21,$24
#calculate
	li $25,66
	sw $23,128($29)
	addu $23,$22,$25
#calculate
	li $24,67
	sw $16,132($29)
	addu $16,$23,$24
#calculate
	li $25,68
	sw $17,136($29)
	addu $17,$16,$25
#calculate
	li $24,69
	sw $18,140($29)
	addu $18,$17,$24
#calculate
	li $25,70
	sw $19,144($29)
	addu $19,$18,$25
#calculate
	li $24,71
	sw $20,148($29)
	addu $20,$19,$24
#calculate
	li $25,72
	sw $21,152($29)
	addu $21,$20,$25
#calculate
	li $24,73
	sw $22,156($29)
	addu $22,$21,$24
#calculate
	li $25,74
	sw $23,160($29)
	addu $23,$22,$25
#calculate
	li $24,75
	sw $16,164($29)
	addu $16,$23,$24
#calculate
	li $25,76
	sw $17,168($29)
	addu $17,$16,$25
#calculate
	li $24,77
	sw $18,172($29)
	addu $18,$17,$24
#calculate
	li $25,78
	sw $19,176($29)
	addu $19,$18,$25
#calculate
	li $24,79
	sw $20,180($29)
	addu $20,$19,$24
#calculate
	li $25,80
	sw $21,184($29)
	addu $21,$20,$25
#calculate
	li $24,81
	sw $22,188($29)
	addu $22,$21,$24
#calculate
	li $25,82
	sw $23,192($29)
	addu $23,$22,$25
#calculate
	li $24,83
	sw $16,196($29)
	addu $16,$23,$24
#calculate
	li $25,84
	sw $17,200($29)
	addu $17,$16,$25
#calculate
	li $24,85
	sw $18,204($29)
	addu $18,$17,$24
#calculate
	li $25,86
	sw $19,208($29)
	addu $19,$18,$25
#calculate
	li $24,87
	sw $20,212($29)
	addu $20,$19,$24
#calculate
	li $25,88
	sw $21,216($29)
	addu $21,$20,$25
#calculate
	li $24,89
	sw $22,220($29)
	addu $22,$21,$24
#calculate
	li $25,90
	sw $23,224($29)
	addu $23,$22,$25
#calculate
	li $24,97
	sw $16,228($29)
	addu $16,$23,$24
#calculate
	li $25,98
	sw $17,232($29)
	addu $17,$16,$25
#calculate
	li $24,99
	sw $18,236($29)
	addu $18,$17,$24
#calculate
	li $25,100
	sw $19,240($29)
	addu $19,$18,$25
#calculate
	li $24,101
	sw $20,244($29)
	addu $20,$19,$24
#calculate
	li $25,102
	sw $21,248($29)
	addu $21,$20,$25
#calculate
	li $24,103
	sw $22,252($29)
	addu $22,$21,$24
#calculate
	li $25,104
	sw $23,256($29)
	addu $23,$22,$25
#calculate
	li $24,105
	sw $16,260($29)
	addu $16,$23,$24
#calculate
	li $25,106
	sw $17,264($29)
	addu $17,$16,$25
#calculate
	li $24,107
	sw $18,268($29)
	addu $18,$17,$24
#calculate
	li $25,108
	sw $19,272($29)
	addu $19,$18,$25
#calculate
	li $24,109
	sw $20,276($29)
	addu $20,$19,$24
#calculate
	li $25,110
	sw $21,280($29)
	addu $21,$20,$25
#calculate
	li $24,111
	sw $22,284($29)
	addu $22,$21,$24
#calculate
	li $25,112
	sw $23,288($29)
	addu $23,$22,$25
#calculate
	li $24,113
	sw $16,292($29)
	addu $16,$23,$24
#calculate
	li $25,114
	sw $17,296($29)
	addu $17,$16,$25
#calculate
	li $24,115
	sw $18,300($29)
	addu $18,$17,$24
#calculate
	li $25,116
	sw $19,304($29)
	addu $19,$18,$25
#calculate
	li $24,117
	sw $20,308($29)
	addu $20,$19,$24
#calculate
	li $25,118
	sw $21,312($29)
	addu $21,$20,$25
#calculate
	li $24,119
	sw $22,316($29)
	addu $22,$21,$24
#calculate
	li $25,120
	sw $23,320($29)
	addu $23,$22,$25
#calculate
	li $24,121
	sw $16,324($29)
	addu $16,$23,$24
#calculate
	li $25,122
	sw $17,328($29)
	addu $17,$16,$25
#print
	li $2,1
	move $a0,$17
	syscall
#printstring
	li $2,4
	la $a0,__string_5
	syscall
#nret
	j $end_BoringGreetings
$end_BoringGreetings:
	lw $31,48($29)
	lw $16,16($29)
	lw $17,20($29)
	lw $18,24($29)
	lw $19,28($29)
	lw $20,32($29)
	lw $21,36($29)
	lw $22,40($29)
	lw $23,44($29)
	addiu $29,$29,372
	jr $31
BoringGreetings2:
	addiu $29,$29,-56
	sw $31,48($29)
	sw $16,16($29)
	sw $17,20($29)
	sw $18,24($29)
	sw $19,28($29)
	sw $20,32($29)
	sw $21,36($29)
	sw $22,40($29)
	sw $23,44($29)
#printstring
	li $2,4
	la $a0,__string_6
	syscall
#ret
	li $24,100
	move $2,$24
	andi $2,$2,255
	j $end_BoringGreetings2
$end_BoringGreetings2:
	lw $31,48($29)
	lw $16,16($29)
	lw $17,20($29)
	lw $18,24($29)
	lw $19,28($29)
	lw $20,32($29)
	lw $21,36($29)
	lw $22,40($29)
	lw $23,44($29)
	addiu $29,$29,56
	jr $31
AllValidCharInt:
	addiu $29,$29,-228
	sw $31,48($29)
	sw $16,16($29)
	sw $17,20($29)
	sw $18,24($29)
	sw $19,28($29)
	sw $20,32($29)
	sw $21,36($29)
	sw $22,40($29)
	sw $23,44($29)
#assigni
	lw $16,248($29)
	li $9,52
	addu $9,$9,$29
	sll $0,$0,2
	addu $9,$9,$0
	sw $16,0($9)
#move
	li $25,1
__label_6:
#lessthan
	lw $17,224($29)
	li $25,10
	bge $17,$25,__label_7
#calculate
	lw $16,224($29)
	li $25,1
	subu $17,$16,$25
#index
	li $9,52
	addu $9,$9,$29
	sll $17,$17,2
	addu $9,$9,$17
	lw $18,0($9)
#calculate
	li $24,1
	addu $19,$18,$24
#assigni
	li $9,52
	addu $9,$9,$29
	sll $16,$16,2
	addu $9,$9,$16
	sw $19,0($9)
#calculate
	li $25,1
	addu $16,$16,$25
#jmp
	j __label_6
__label_7:
#move
__label_8:
#lessthan
	li $24,10
	bge $16,$24,__label_9
#index
	li $9,52
	addu $9,$9,$29
	lw $16,224($29)
	sll $16,$16,2
	addu $9,$9,$16
	lw $17,0($9)
#printstring
	li $2,4
	la $a0,__string_7
	syscall
#print
	li $2,1
	move $a0,$17
	syscall
#calculate
	li $25,1
	addu $16,$16,$25
#jmp
	j __label_8
__label_9:
#move
#move
	lw $18,236($29)
#calculate
	lw $19,248($29)
	lw $20,240($29)
	div $21,$19,$20
#calculate
	lw $22,244($29)
	addu $25,$22,$21
	sw $25,__glbl_var_p
#move
	lw $23,228($29)
#printstring
	li $2,4
	la $a0,__string_8
	syscall
__label_10:
#lessthan
	sw $16,224($29)
	lw $16,216($29)
	sw $17,108($29)
	lw $17,232($29)
	bgt $16,$17,__label_11
#calculate
	lw $16,216($29)
	addu $17,$16,$0
#notequal
	li $24,34
	beq $17,$24,__label_12
#calculate
	lw $16,216($29)
	addu $17,$16,$0
#greaterequal
	li $24,32
	blt $17,$24,__label_13
#calculate
	lw $16,216($29)
	li $24,95
	subu $17,$16,$24
#calculate
	li $25,95
	addu $18,$17,$25
#bne
	li $24,95
	bne $18,$24,__label_14
#move
	li $25,1
#jmp
	j __label_26
__label_14:
#bne
	lw $16,144($29)
	li $24,43
	bne $16,$24,__label_15
#move
	li $25,1
#jmp
	j __label_26
__label_15:
#bne
	lw $16,144($29)
	li $24,45
	bne $16,$24,__label_16
#move
	li $25,1
#jmp
	j __label_26
__label_16:
#bne
	lw $16,144($29)
	li $24,42
	bne $16,$24,__label_17
#move
	li $25,1
#jmp
	j __label_26
__label_17:
#bne
	lw $16,144($29)
	li $24,47
	bne $16,$24,__label_18
#move
	li $25,1
#jmp
	j __label_26
__label_18:
#calculate
	lw $16,216($29)
	li $25,48
	subu $17,$16,$25
#greaterequal
	blt $17,$0,__label_20
#calculate
	lw $16,216($29)
	li $25,57
	subu $17,$16,$25
#lessthan
	bgt $17,$0,__label_21
#move
	li $24,1
__label_21:
__label_20:
#calculate
	lw $16,216($29)
	li $24,65
	subu $17,$16,$24
#greaterequal
	blt $17,$0,__label_22
#calculate
	lw $16,216($29)
	li $24,90
	subu $17,$16,$24
#lessthan
	bgt $17,$0,__label_23
#move
	li $25,1
__label_23:
__label_22:
#calculate
	lw $16,216($29)
	li $25,97
	subu $17,$16,$25
#greaterequal
	blt $17,$0,__label_24
#calculate
	lw $16,216($29)
	li $25,122
	subu $17,$16,$25
#lessthan
	bgt $17,$0,__label_25
#move
	li $24,1
__label_25:
__label_24:
__label_26:
__label_13:
#calculate
	lw $16,216($29)
	lw $17,228($29)
	subu $18,$16,$17
#assignc
	sb $16,__glbl_array_str($18)
#equalto
	lw $19,220($29)
	li $24,1
	bne $19,$24,__label_27
#calculate
	lw $16,216($29)
	lw $17,228($29)
	subu $18,$16,$17
#index
	la $9,__glbl_array_str
	addu $9,$9,$18
	lb $19,0($9)
#printstring
	li $2,4
	la $a0,__string_9
	syscall
#print
	li $2,11
	move $a0,$19
	syscall
#move
__label_27:
__label_12:
#calculate
	addu $20,$16,$0
#calculate
	li $25,34
	li $24,-34
	addu $21,$25,$24
#calculate
	li $25,34
	addu $22,$21,$25
#equalto
	bne $20,$22,__label_28
#printstring
	li $2,4
	la $a0,__string_10
	syscall
__label_28:
#move
	li $24,99
#jmp
	j __label_10
__label_11:
#calculate
	lw $25,__glbl_var_p
	li $24,1
	addu $25,$25,$24
	sw $25,__glbl_var_p
#move
	li $24,97
	sb $24,__glbl_var_y
#calculate
	lw $25,__glbl_var_y
	li $24,1
	addu $25,$25,$24
	sw $25,__glbl_var_p
#move
	li $24,98
	sb $24,__glbl_var_z
#calculate
	lw $25,__glbl_var_z
	li $24,2
	addu $25,$25,$24
	sw $25,__glbl_var_p
#move
	li $24,99
	sb $24,__glbl_var_z
#move
	li $25,99
	sb $25,__glbl_var_y
#nret
	j $end_AllValidCharInt
$end_AllValidCharInt:
	lw $31,48($29)
	lw $16,16($29)
	lw $17,20($29)
	lw $18,24($29)
	lw $19,28($29)
	lw $20,32($29)
	lw $21,36($29)
	lw $22,40($29)
	lw $23,44($29)
	addiu $29,$29,228
	jr $31
RecursionGCD:
	addiu $29,$29,-72
	sw $31,48($29)
	sw $16,16($29)
	sw $17,20($29)
	sw $18,24($29)
	sw $19,28($29)
	sw $20,32($29)
	sw $21,36($29)
	sw $22,40($29)
	sw $23,44($29)
#equalto
	lw $16,76($29)
	bne $16,$0,__label_29
#ret
	lw $16,72($29)
	move $2,$16
	j $end_RecursionGCD
__label_29:
#notequal
	lw $17,76($29)
	beq $17,$0,__label_30
#move
	lw $16,76($29)
	move $17,$16
#calculate
	lw $18,72($29)
	div $19,$18,$16
#calculate
	mul $20,$19,$16
#calculate
	subu $21,$18,$20
#push
	sw $17,0($29)
#push
	sw $21,4($29)
#call
	jal RecursionGCD
#result
	move $22,$2
#ret
	move $2,$22
	j $end_RecursionGCD
__label_30:
$end_RecursionGCD:
	lw $31,48($29)
	lw $16,16($29)
	lw $17,20($29)
	lw $18,24($29)
	lw $19,28($29)
	lw $20,32($29)
	lw $21,36($29)
	lw $22,40($29)
	lw $23,44($29)
	addiu $29,$29,72
	jr $31
main:
	addiu $29,$29,-120
	sw $31,60($29)
#printstring
	li $2,4
	la $a0,__string_11
	syscall
#readi
	li $2,5
	syscall
	move $16,$2
#readi
	li $2,5
	syscall
	move $17,$2
#printstring
	li $2,4
	la $a0,__string_12
	syscall
#print
	li $2,1
	move $a0,$16
	syscall
#print
	li $2,1
	move $a0,$17
	syscall
#move
	move $18,$16
#move
	move $19,$17
#push
	sw $18,0($29)
#push
	sw $19,4($29)
#call
	jal RecursionGCD
#result
	move $20,$2
#printstring
	li $2,4
	la $a0,__string_13
	syscall
#print
	li $2,1
	move $a0,$16
	syscall
#print
	li $2,1
	move $a0,$17
	syscall
#printstring
	li $2,4
	la $a0,__string_14
	syscall
#print
	li $2,1
	move $a0,$20
	syscall
#bne
	li $25,100
	bne $16,$25,__label_31
#printstring
	li $2,4
	la $a0,__string_15
	syscall
#call
	jal BoringGreetings
#call
	jal BoringGreetings2
#result
	move $16,$2
#printstring
	li $2,4
	la $a0,__string_16
	syscall
#print
	li $2,1
	move $a0,$16
	syscall
#push
	li $24,97
	sb $24,0($29)
#push
	li $25,98
	sb $25,4($29)
#push
	li $24,9
	sw $24,8($29)
#push
	li $25,10
	sw $25,12($29)
#push
	li $24,11
	sw $24,16($29)
#push
	li $25,12
	sw $25,20($29)
#push
	li $24,13
	sw $24,24($29)
#call
	jal AllValidCharInt
#jmp
	j __label_34
__label_31:
#bne
	lw $17,108($29)
	li $24,-1
	bne $17,$24,__label_32
#printstring
	li $2,4
	la $a0,__string_17
	syscall
#jmp
	j __label_34
__label_32:
__label_34:
#nret
	j $end_main
$end_main:
	lw $31,60($29)
	addiu $29,$29,120
	jr $31
