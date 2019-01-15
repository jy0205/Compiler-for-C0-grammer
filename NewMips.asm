.data
const_char_a:.word 97
const_char_b:.word 98
const_char_c:.word 99
const_char_A:.word 65
const_int_postive:.word 1
const_int_zero:.word 0
const_int_negtive:.word -1
const_char_num:.word 57
const_char_underline:.word 95
const_char_add:.word 43
const_char_mul:.word 42
int_global_var:.word 0
char_global_var:.word 0
sort:.space 60
string135:.asciiz "2 <= 1"
string138:.asciiz "1 == 2"
string173:.asciiz "1 - 1 = true"
string416:.asciiz " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
string417:.asciiz "int_test_input = "
string418:.asciiz "char_test_input = "
string583:.asciiz "\n"

.text
j main
testIfWhile:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,412
li $t0,98
li $t1,99
li $s0,0
li $s1,0
ble $t1,$t0,$label0
$label1:
li $v1,100
bge $s0,$v1,$label2
move $t0,$s1
addiu $s1,$t0,1
addu $s0,$s0,$t0
j $label1
$label2:
subiu $t0,$s0,100
li $v1,3
mult $t0,$v1
mflo $t0
li $v1,15
bne $t0,$v1,$label4
li $v0,2
mult $v0,$s1
mflo $t0
li $v1,10
mult $s0,$v1
mflo $t1
li $v0,0
subu $t1,$v0,$t1
addu $t0,$t1,$t0
addiu $t0,$t0,117
subiu $s0,$t0,98
li $v0,2
mult $v0,$s0
mflo $t0
li $t1,970
subu $t2,$s1,$s0
addu $t1,$t2,$t1
subiu $t1,$t1,100
addiu $t1,$t1,42
addiu $t1,$t1,34
addu $s1,$t1,$t0
j $label3
$label4:
li $v0,5
mult $v0,$s1
mflo $t0
li $v1,4
mult $s0,$v1
mflo $t1
addu $t0,$t1,$t0
addiu $t0,$t0,24
subiu $t0,$t0,97
addiu $s0,$t0,99
li $v0,4
mult $v0,$s0
mflo $t0
li $t1,388
li $v0,0
subu $t2,$v0,$s1
addu $t2,$t2,$s0
addu $t1,$t2,$t1
subiu $t1,$t1,1653
addiu $t1,$t1,453
addiu $t1,$t1,555
subu $s1,$t1,$t0
$label3:
$label0:
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$s1
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $t0,98
li $t1,99
li $s0,0
li $s1,0
blt $t1,$t0,$label5
$label6:
li $v1,10
bge $s0,$v1,$label7
li $v1,2
ble $s1,$v1,$label8
addu $s0,$s0,$s1
$label8:
li $v1,2
div $s0,$v1
mflo $t0
li $v1,2
mult $t0,$v1
mflo $t0
addiu $s1,$s1,1
bne $t0,$s0,$label9
addiu $s1,$s1,1
$label9:
j $label6
$label7:
$label5:
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$s1
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $s0,0
li $s1,0
li $v0,1
li $v1,2
bge $v0,$v1,$label10
$label11:
li $v1,20
bge $s0,$v1,$label12
li $v1,2
div $s0,$v1
mflo $t0
li $v1,2
mult $t0,$v1
mflo $t0
subu $t0,$s0,$t0
li $v1,0
bne $t0,$v1,$label14
addiu $s0,$s0,1
j $label13
$label14:
li $v1,1
bne $t0,$v1,$label15
addu $s0,$s0,$s1
j $label13
$label15:
$label13:
addiu $s1,$s1,1
j $label11
$label12:
li $v1,2
div $s0,$v1
mflo $t0
li $v1,2
mult $t0,$v1
mflo $t0
bne $s0,$t0,$label16
li $v0,2
mult $v0,$s1
mflo $t0
li $v1,7
mult $s0,$v1
mflo $t1
addu $t0,$t1,$t0
addiu $t0,$t0,99
subiu $t0,$t0,97
addiu $s0,$t0,103
li $v0,2
mult $v0,$s0
mflo $t0
li $t1,194
li $v0,0
subu $t2,$v0,$s1
addu $t2,$t2,$s0
addu $t1,$t2,$t1
subiu $t1,$t1,523
addiu $t1,$t1,53
addiu $t1,$t1,355
subu $s1,$t1,$t0
$label16:
$label10:
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$s1
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $v0,2
li $v1,1
bgt $v0,$v1,$label17
la $a0,string135
li $v0,4
syscall
la $a0,string583
li $v0,4
syscall
$label17:
li $v0,1
li $v1,2
bne $v0,$v1,$label18
la $a0,string138
li $v0,4
syscall
la $a0,string583
li $v0,4
syscall
$label18:
li $s0,0
li $s1,0
li $v0,1
li $v1,2
beq $v0,$v1,$label19
li $v1,2
mult $s1,$v1
mflo $t0
subu $t0,$s0,$t0
li $v1,0
bne $t0,$v1,$label21
$label22:
li $v1,30
bge $s0,$v1,$label23
li $v1,2
div $s0,$v1
mflo $t0
li $v1,2
mult $t0,$v1
mflo $t0
subu $t0,$t0,$s0
li $v1,-1
bne $t0,$v1,$label25
addiu $s0,$s0,1
j $label24
$label25:
li $v1,0
bne $t0,$v1,$label26
addu $s0,$s0,$s1
j $label24
$label26:
$label24:
addiu $s1,$s1,1
j $label22
$label23:
j $label20
$label21:
li $s0,-1
move $s1,$s0
$label20:
$label19:
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$s1
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $t0,0
beq $t0,0,$label27
la $a0,string173
li $v0,4
syscall
la $a0,string583
li $v0,4
syscall
$label27:
li $v1,-1
sw $v1,int_global_var
li $v0,97
sw $v0,char_global_var
lw $ra,0($fp)
jr $ra
testSwitchCase:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,100
li $v1,-1
bne $a0,$v1,$label29
$label30:
li $v1,20
mult $a0,$v1
mflo $t0
addiu $t0,$t0,6
li $v1,10
bge $t0,$v1,$label31
li $v1,0
bgt $a0,$v1,$label32
addiu $a0,$a0,1
$label32:
j $label30
$label31:
j $label28
$label29:
li $v1,0
bne $a0,$v1,$label33
li $t0,-1
subu $t0,$a0,$t0
li $v1,0
bne $t0,$v1,$label34
$label35:
li $v1,0
bgt $a0,$v1,$label36
addiu $t0,$a0,1
li $v1,-2
mult $t0,$v1
mflo $a0
j $label35
$label36:
$label34:
j $label28
$label33:
li $v1,1
bne $a0,$v1,$label37
li $a0,-233
j $label28
$label37:
li $a0,233
$label28:
li $v1,97
bne $a1,$v1,$label39
li $a1,97
j $label38
$label39:
li $v1,65
bne $a1,$v1,$label40
li $a1,65
j $label38
$label40:
li $v1,57
bne $a1,$v1,$label41
li $a1,57
j $label38
$label41:
li $v1,95
bne $a1,$v1,$label42
li $a1,95
j $label38
$label42:
li $v1,43
bne $a1,$v1,$label43
li $a1,43
j $label38
$label43:
li $v1,42
bne $a1,$v1,$label44
li $a1,42
j $label38
$label44:
li $a1,119
$label38:
li $v0,0
li $v1,0
bne $v0,$v1,$label46
j $label45
$label46:
$label45:
move $ra,$a0
move $a0,$ra
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$ra
move $ra,$a0
move $a0,$a1
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$ra
li $v1,-2
sw $v1,int_global_var
li $v0,98
sw $v0,char_global_var
lw $ra,0($fp)
jr $ra
returnInt:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $v0,10
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnChar:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $v0,114
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnIntParameter:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,36
li $v1,1
bne $a0,$v1,$label48
move $s0,$a1
j $label47
$label48:
li $v1,2
bne $a0,$v1,$label49
move $s0,$a2
j $label47
$label49:
li $v1,3
bne $a0,$v1,$label50
move $s0,$a3
j $label47
$label50:
li $v1,4
bne $a0,$v1,$label51
lw $v1,-24($fp)
move $s0,$v1
j $label47
$label51:
li $s0,0
$label47:
move $v0,$s0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnCharParameter:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
li $v1,1
bne $a0,$v1,$label53
move $v0,$a1
lw $ra,0($fp)
jr $ra
j $label52
$label53:
li $v1,2
bne $a0,$v1,$label54
move $v0,$a2
lw $ra,0($fp)
jr $ra
j $label52
$label54:
li $v1,3
bne $a0,$v1,$label55
move $v0,$a3
lw $ra,0($fp)
jr $ra
j $label52
$label55:
li $v1,4
bne $a0,$v1,$label56
lw $v1,-24($fp)
move $v0,$v1
lw $ra,0($fp)
jr $ra
j $label52
$label56:
li $v0,48
lw $ra,0($fp)
jr $ra
$label52:
lw $ra,0($fp)
jr $ra
testAssign:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,272
li $t0,-2
li $t1,1
sw $t0,-48($fp)
sw $t1,-44($fp)
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-48($fp)
lw $t1,-44($fp)
move $t2,$v0
subiu $t2,$t2,97
li $a0,2
move $a1,$t1
move $a2,$t0
li $a3,10
sw $t2,-24($sp)
jal returnIntParameter
move $sp,$fp
lw $fp,-4($fp)
move $t0,$v0
li $t1,2
mult $t0,$t1
mflo $t0
li $v1,2
div $t0,$v1
mflo $t0
li $t1,2
li $v0,10
addu $t0,$v0,$t0
move $s0,$t0
li $v1,4
div $t0,$v1
mflo $t0
addiu $t0,$t0,97
subiu $t0,$t0,97
subiu $t0,$t0,2
sll $v0,$t0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
sw $t1,0($fp)
addu $fp,$fp,$v0
li $v0,0
sll $v0,$v0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
div $s0,$t0
mflo $t0
li $v0,-2
addu $t0,$v0,$t0
sw $t0,-120($fp)
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-120($fp)
move $t1,$v0
addu $t0,$t0,$t1
subiu $t0,$t0,11
sw $t0,-132($fp)
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-132($fp)
move $t1,$v0
li $v0,0
subu $t1,$v0,$t1
sw $t0,-132($fp)
sw $t1,-140($fp)
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-132($fp)
lw $t1,-140($fp)
move $t2,$v0
subiu $t2,$t2,97
li $t3,-98
addiu $t3,$t3,99
li $v0,10
mult $v0,$t2
mflo $t2
sw $t0,-132($fp)
sw $t1,-140($fp)
sw $t2,-156($fp)
sw $t3,-164($fp)
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-132($fp)
lw $t1,-140($fp)
lw $t2,-156($fp)
lw $t3,-164($fp)
move $t4,$v0
li $a0,4
move $a1,$t3
move $a2,$t4
li $a3,10
li $v0,97
sw $v0,-24($sp)
sw $s0,-12($fp)
sw $t0,-132($fp)
sw $t1,-140($fp)
sw $t2,-156($fp)
jal returnIntParameter
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-12($fp)
lw $t0,-132($fp)
lw $t1,-140($fp)
lw $t2,-156($fp)
move $t3,$v0
addu $t2,$t2,$t3
addu $t1,$t1,$t2
sll $v0,$t0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
sw $t1,0($fp)
addu $fp,$fp,$v0
li $t0,-255
li $v0,1
sll $v0,$v0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
lw $t1,0($fp)
addu $fp,$fp,$v0
li $t2,196
li $t3,-98
addiu $t3,$t3,99
addu $t0,$t0,$t1
addu $t0,$t0,$t2
subiu $t0,$t0,196
sll $v0,$t0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
sw $t3,0($fp)
addu $fp,$fp,$v0
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
move $t0,$v0
move $s1,$t0
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
move $t0,$v0
sw $t0,-220($fp)
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-220($fp)
move $t1,$v0
li $t2,97
li $a0,4
li $a1,97
move $a2,$t0
move $a3,$t1
sw $t2,-24($sp)
jal returnCharParameter
move $sp,$fp
lw $fp,-4($fp)
move $t0,$v0
li $v0,0
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
sw $t0,0($fp)
addu $fp,$fp,$v0
li $v0,0
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
li $v0,1
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
sw $t0,0($fp)
addu $fp,$fp,$v0
li $v1,120
li $v0,2
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
sw $v1,0($fp)
addu $fp,$fp,$v0
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $v0,0
sll $v0,$v0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $v0,1
sll $v0,$v0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
li $v0,2
sll $v0,$v0,2
addiu $v0,$v0,16
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
move $a0,$s1
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
li $v0,0
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
li $v0,1
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
li $v0,2
sll $v0,$v0,2
addiu $v0,$v0,32
subu $fp,$fp,$v0
lw $t0,0($fp)
addu $fp,$fp,$v0
move $a0,$t0
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
li $t0,-3
sw $t0,int_global_var
li $v0,99
sw $v0,char_global_var
lw $ra,0($fp)
jr $ra
testInputOutput:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
lw $s0,-8($fp)
lw $s1,-12($fp)
li $v0,5
syscall
move $s0,$v0
li $v0,12
syscall
move $s1,$v0
la $a0,string416
li $v0,4
syscall
la $a0,string583
li $v0,4
syscall
la $a0,string417
li $v0,4
syscall
move $a0,$s0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
la $a0,string418
li $v0,4
syscall
move $a0,$s1
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
li $t0,-4
move $s2,$t0
li $s3,100
lw $ra,0($fp)
jr $ra
testTrick:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
lw $ra,0($fp)
jr $ra
judge1:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
li $s0,0
bge $a0,$a1,$label57
blt $a2,$a3,$label58
li $s0,1
$label58:
$label57:
move $v0,$s0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
judge2:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
li $s0,0
bge $a0,$a1,$label59
bgt $a2,$a3,$label60
li $s0,1
$label60:
$label59:
move $v0,$s0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
Qsort:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,84
move $s0,$a0
move $s1,$a1
sll $v0,$s0,2
lw $t0,sort($v0)
move $s2,$t0
blt $a0,$a1,$label61
lw $ra,0($fp)
jr $ra
$label61:
$label62:
bge $s0,$s1,$label63
$label64:
sll $v0,$s1,2
lw $t0,sort($v0)
sw $a0,-8($fp)
sw $a1,-12($fp)
move $a0,$s0
move $a1,$s1
move $a2,$t0
move $a3,$s2
sw $s0,-16($fp)
jal judge1
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-16($fp)
lw $a0,-8($fp)
lw $a1,-12($fp)
move $t0,$v0
li $v1,1
bne $t0,$v1,$label65
subiu $s1,$s1,1
j $label64
$label65:
sll $v0,$s1,2
lw $t0,sort($v0)
sll $v0,$s0,2
sw $t0,sort($v0)
$label66:
sll $v0,$s0,2
lw $t0,sort($v0)
sw $a0,-8($fp)
sw $a1,-12($fp)
move $a0,$s0
move $a1,$s1
move $a2,$t0
move $a3,$s2
sw $s0,-16($fp)
jal judge2
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-16($fp)
lw $a0,-8($fp)
lw $a1,-12($fp)
move $t0,$v0
li $v1,1
bne $t0,$v1,$label67
addiu $s0,$s0,1
j $label66
$label67:
sll $v0,$s0,2
lw $t0,sort($v0)
sll $v0,$s1,2
sw $t0,sort($v0)
j $label62
$label63:
sll $v0,$s0,2
sw $s2,sort($v0)
subiu $t0,$s0,1
sw $a0,-8($fp)
sw $a1,-12($fp)
move $a0,$a0
move $a1,$t0
sw $s0,-16($fp)
sw $s1,-20($fp)
sw $s2,-24($fp)
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-16($fp)
lw $s1,-20($fp)
lw $s2,-24($fp)
lw $a0,-8($fp)
lw $a1,-12($fp)
addiu $t0,$s0,1
sw $a0,-8($fp)
sw $a1,-12($fp)
move $a0,$t0
lw $v1,-12($fp)
move $a1,$v1
sw $s0,-16($fp)
sw $s1,-20($fp)
sw $s2,-24($fp)
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-16($fp)
lw $s1,-20($fp)
lw $s2,-24($fp)
lw $a0,-8($fp)
lw $a1,-12($fp)
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
testRecursion:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
li $v1,5
li $v0,0
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,8
li $v0,1
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,3
li $v0,2
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,6
li $v0,3
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,4
li $v0,4
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,1
li $v0,5
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,2
li $v0,6
sll $v0,$v0,2
sw $v1,sort($v0)
li $v1,7
li $v0,7
sll $v0,$v0,2
sw $v1,sort($v0)
li $a0,0
li $a1,7
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
li $s0,0
$label68:
li $v1,8
bge $s0,$v1,$label69
sll $v0,$s0,2
lw $t0,sort($v0)
move $a0,$t0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
addiu $s0,$s0,1
j $label68
$label69:
li $t0,-5
sw $t0,int_global_var
li $v0,101
sw $v0,char_global_var
lw $ra,0($fp)
jr $ra
testCallFunction:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,36
lw $s0,-8($fp)
lw $s1,-4($fp)
lw $s2,-4($fp)
li $v0,5
syscall
move $s0,$v0
li $v1,0
bne $s0,$v1,$label71
sw $s0,-8($fp)
sw $s1,-4($fp)
jal testIfWhile
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-8($fp)
lw $s1,-4($fp)
j $label70
$label71:
li $v1,1
bne $s0,$v1,$label72
li $t0,1
li $t1,97
move $a0,$t0
move $a1,$t1
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
li $t0,-1
move $a0,$t0
li $a1,43
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
li $a0,0
li $a1,95
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
li $a0,2
li $a1,57
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
j $label70
$label72:
li $v1,2
bne $s0,$v1,$label73
sw $s0,-8($fp)
sw $s1,-4($fp)
sw $s2,-4($fp)
jal testAssign
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-8($fp)
lw $s1,-4($fp)
lw $s2,-4($fp)
j $label70
$label73:
li $v1,3
bne $s0,$v1,$label74
sw $s0,-8($fp)
sw $s1,-4($fp)
sw $s2,-4($fp)
jal testInputOutput
move $sp,$fp
lw $fp,-4($fp)
lw $s0,-8($fp)
lw $s1,-4($fp)
lw $s2,-4($fp)
j $label70
$label74:
li $v1,4
bne $s0,$v1,$label75
sw $s1,-4($fp)
sw $s2,-4($fp)
jal testRecursion
move $sp,$fp
lw $fp,-4($fp)
lw $s1,-4($fp)
lw $s2,-4($fp)
j $label70
$label75:
$label70:
lw $v1,int_global_var
move $t0,$v1
move $a0,$t0
li $v0,1
syscall
la $a0,string583
li $v0,4
syscall
lw $v1,char_global_var
move $t0,$v1
move $a0,$t0
li $v0,11
syscall
la $a0,string583
li $v0,4
syscall
lw $ra,0($fp)
jr $ra
main:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $v0,0
sw $v0,int_global_var
li $v0,95
sw $v0,char_global_var
jal testCallFunction
move $sp,$fp
lw $fp,-4($fp)
jal testTrick
move $sp,$fp
lw $fp,-4($fp)
