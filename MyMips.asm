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
string148:.asciiz "2 <= 1"
string151:.asciiz "1 == 2"
string191:.asciiz "1 - 1 = true"
string451:.asciiz " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
string452:.asciiz "int_test_input = "
string453:.asciiz "char_test_input = "
string621:.asciiz "\n"

.text
j main
testIfWhile:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,344
li $t0,1
sw $t0,-8($fp)
li $t0,2
sw $t0,-12($fp)
li $t0,98
sw $t0,-16($fp)
li $t0,97
sw $t0,-20($fp)
li $t0,0
sw $t0,-24($fp)
li $t1,0
sw $t1,-28($fp)
lw $t2,-16($fp)
li $t3,1
addu $t4,$t2,$t3
sw $t4,-32($fp)
lw $t5,-20($fp)
li $t6,1
addu $t7,$t5,$t6
sw $t7,-36($fp)
ble $t4,$t7,$label0
sw $t2,-16($fp)
sw $t4,-32($fp)
sw $t5,-20($fp)
sw $t7,-36($fp)
$label1:
lw $t0,-24($fp)
li $t1,100
bge $t0,$t1,$label2
lw $t2,-28($fp)
addu $t3,$t0,$t2
sw $t3,-40($fp)
sw $t3,-24($fp)
move $t0,$t3
li $t4,1
addu $t5,$t2,$t4
sw $t5,-44($fp)
sw $t5,-28($fp)
move $t2,$t5
j $label1
sw $t0,-24($fp)
sw $t2,-28($fp)
sw $t3,-40($fp)
sw $t5,-44($fp)
$label2:
lw $t0,-24($fp)
li $t1,100
subu $t2,$t0,$t1
sw $t2,-48($fp)
li $t3,3
mult $t2,$t3
mflo $t4
sw $t4,-52($fp)
li $t5,15
bne $t4,$t5,$label4
li $t6,10
mult $t0,$t6
mflo $t7
sw $t7,-56($fp)
li $t8,0
subu $t9,$t8,$t7
sw $t9,-60($fp)
sw $t0,-24($fp)
li $t0,2
lw $t1,-28($fp)
sw $t2,-48($fp)
mult $t0,$t1
mflo $t2
sw $t2,-64($fp)
sw $t2,-64($fp)
addu $t2,$t9,$t2
sw $t2,-68($fp)
li $t0,117
sw $t2,-68($fp)
addu $t2,$t2,$t0
sw $t2,-72($fp)
lw $t0,-16($fp)
sw $t2,-72($fp)
subu $t2,$t2,$t0
sw $t2,-76($fp)
sw $t2,-24($fp)
sw $t0,-16($fp)
lw $t0,-24($fp)
sw $t2,-76($fp)
subu $t2,$t1,$t0
sw $t2,-80($fp)
sw $t0,-24($fp)
li $t0,10
sw $t1,-28($fp)
lw $t1,-20($fp)
sw $t2,-80($fp)
mult $t0,$t1
mflo $t2
sw $t2,-84($fp)
lw $t0,-80($fp)
sw $t2,-84($fp)
addu $t2,$t0,$t2
sw $t2,-88($fp)
sw $t0,-80($fp)
li $t0,100
sw $t2,-88($fp)
subu $t2,$t2,$t0
sw $t2,-92($fp)
li $t0,42
sw $t2,-92($fp)
addu $t2,$t2,$t0
sw $t2,-96($fp)
li $t0,34
sw $t2,-96($fp)
addu $t2,$t2,$t0
sw $t2,-100($fp)
li $t0,2
sw $t1,-20($fp)
lw $t1,-24($fp)
sw $t2,-100($fp)
mult $t0,$t1
mflo $t2
sw $t2,-104($fp)
lw $t0,-100($fp)
sw $t2,-104($fp)
addu $t2,$t0,$t2
sw $t2,-108($fp)
sw $t2,-28($fp)
j $label3
sw $t0,-100($fp)
sw $t1,-24($fp)
sw $t2,-108($fp)
sw $t4,-52($fp)
sw $t7,-56($fp)
sw $t9,-60($fp)
$label4:
lw $t0,-24($fp)
li $t1,4
mult $t0,$t1
mflo $t2
sw $t2,-112($fp)
li $t3,5
lw $t4,-28($fp)
mult $t3,$t4
mflo $t5
sw $t5,-116($fp)
addu $t6,$t2,$t5
sw $t6,-120($fp)
li $t7,24
addu $t8,$t6,$t7
sw $t8,-124($fp)
lw $t9,-20($fp)
sw $t2,-112($fp)
subu $t2,$t8,$t9
sw $t2,-128($fp)
sw $t0,-24($fp)
li $t0,99
sw $t2,-128($fp)
addu $t2,$t2,$t0
sw $t2,-132($fp)
sw $t2,-24($fp)
li $t0,0
sw $t2,-132($fp)
subu $t2,$t0,$t4
sw $t2,-136($fp)
lw $t0,-24($fp)
sw $t2,-136($fp)
addu $t2,$t2,$t0
sw $t2,-140($fp)
sw $t0,-24($fp)
li $t0,4
sw $t2,-140($fp)
mult $t0,$t9
mflo $t2
sw $t2,-144($fp)
lw $t0,-140($fp)
sw $t2,-144($fp)
addu $t2,$t0,$t2
sw $t2,-148($fp)
sw $t0,-140($fp)
li $t0,1653
sw $t2,-148($fp)
subu $t2,$t2,$t0
sw $t2,-152($fp)
li $t0,453
sw $t2,-152($fp)
addu $t2,$t2,$t0
sw $t2,-156($fp)
li $t0,555
sw $t2,-156($fp)
addu $t2,$t2,$t0
sw $t2,-160($fp)
li $t0,4
lw $t1,-24($fp)
sw $t2,-160($fp)
mult $t0,$t1
mflo $t2
sw $t2,-164($fp)
lw $t0,-160($fp)
sw $t2,-164($fp)
subu $t2,$t0,$t2
sw $t2,-168($fp)
sw $t2,-28($fp)
move $t4,$t2
sw $t0,-160($fp)
sw $t1,-24($fp)
sw $t2,-168($fp)
sw $t4,-28($fp)
sw $t5,-116($fp)
sw $t6,-120($fp)
sw $t8,-124($fp)
sw $t9,-20($fp)
$label3:
$label0:
lw $t0,-24($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t1,-28($fp)
move $a0,$t1
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
li $t2,0
sw $t2,-24($fp)
move $t0,$t2
li $t3,0
sw $t3,-28($fp)
move $t1,$t3
lw $t4,-16($fp)
li $t5,1
addu $t6,$t4,$t5
sw $t6,-172($fp)
lw $t7,-20($fp)
li $t8,1
addu $t9,$t7,$t8
sw $t9,-176($fp)
blt $t6,$t9,$label5
sw $t0,-24($fp)
sw $t1,-28($fp)
sw $t4,-16($fp)
sw $t6,-172($fp)
sw $t7,-20($fp)
sw $t9,-176($fp)
$label6:
lw $t0,-24($fp)
li $t1,10
bge $t0,$t1,$label7
lw $t2,-28($fp)
li $t3,2
ble $t2,$t3,$label8
addu $t4,$t0,$t2
sw $t4,-180($fp)
sw $t4,-24($fp)
move $t0,$t4
sw $t0,-24($fp)
sw $t2,-28($fp)
sw $t4,-180($fp)
$label8:
lw $t0,-28($fp)
li $t1,1
addu $t2,$t0,$t1
sw $t2,-184($fp)
sw $t2,-28($fp)
move $t0,$t2
lw $t3,-24($fp)
li $t4,2
div $t3,$t4
mflo $t5
sw $t5,-188($fp)
li $t6,2
mult $t5,$t6
mflo $t7
sw $t7,-192($fp)
bne $t7,$t3,$label9
li $t8,1
addu $t9,$t0,$t8
sw $t9,-196($fp)
sw $t9,-28($fp)
move $t0,$t9
sw $t0,-28($fp)
sw $t2,-184($fp)
sw $t3,-24($fp)
sw $t5,-188($fp)
sw $t7,-192($fp)
sw $t9,-196($fp)
$label9:
j $label6
$label7:
$label5:
lw $t0,-24($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t1,-28($fp)
move $a0,$t1
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
li $t2,0
sw $t2,-24($fp)
move $t0,$t2
li $t3,0
sw $t3,-28($fp)
move $t1,$t3
lw $t4,-8($fp)
lw $t5,-12($fp)
bge $t4,$t5,$label10
sw $t0,-24($fp)
sw $t1,-28($fp)
sw $t4,-8($fp)
sw $t5,-12($fp)
$label11:
lw $t0,-24($fp)
li $t1,20
bge $t0,$t1,$label12
li $t2,2
div $t0,$t2
mflo $t3
sw $t3,-200($fp)
li $t4,2
mult $t3,$t4
mflo $t5
sw $t5,-204($fp)
subu $t6,$t0,$t5
sw $t6,-208($fp)
li $t7,0
bne $t6,$t7,$label14
li $t8,1
addu $t9,$t0,$t8
sw $t9,-212($fp)
sw $t9,-24($fp)
move $t0,$t9
j $label13
sw $t0,-24($fp)
sw $t3,-200($fp)
sw $t5,-204($fp)
sw $t6,-208($fp)
sw $t9,-212($fp)
$label14:
lw $t0,-208($fp)
li $t1,1
bne $t0,$t1,$label15
lw $t2,-24($fp)
lw $t3,-28($fp)
addu $t4,$t2,$t3
sw $t4,-216($fp)
sw $t4,-24($fp)
move $t2,$t4
j $label13
sw $t0,-208($fp)
sw $t2,-24($fp)
sw $t3,-28($fp)
sw $t4,-216($fp)
$label15:
$label13:
lw $t0,-28($fp)
li $t1,1
addu $t2,$t0,$t1
sw $t2,-220($fp)
sw $t2,-28($fp)
move $t0,$t2
j $label11
sw $t0,-28($fp)
sw $t2,-220($fp)
$label12:
lw $t0,-24($fp)
li $t1,2
div $t0,$t1
mflo $t2
sw $t2,-224($fp)
li $t3,2
mult $t2,$t3
mflo $t4
sw $t4,-228($fp)
bne $t0,$t4,$label16
li $t5,7
mult $t0,$t5
mflo $t6
sw $t6,-232($fp)
li $t7,2
lw $t8,-28($fp)
mult $t7,$t8
mflo $t9
sw $t9,-236($fp)
sw $t2,-224($fp)
addu $t2,$t6,$t9
sw $t2,-240($fp)
sw $t0,-24($fp)
li $t0,99
sw $t2,-240($fp)
addu $t2,$t2,$t0
sw $t2,-244($fp)
lw $t0,-20($fp)
sw $t2,-244($fp)
subu $t2,$t2,$t0
sw $t2,-248($fp)
sw $t0,-20($fp)
li $t0,103
sw $t2,-248($fp)
addu $t2,$t2,$t0
sw $t2,-252($fp)
sw $t2,-24($fp)
li $t0,0
sw $t2,-252($fp)
subu $t2,$t0,$t8
sw $t2,-256($fp)
lw $t0,-24($fp)
sw $t2,-256($fp)
addu $t2,$t2,$t0
sw $t2,-260($fp)
sw $t0,-24($fp)
li $t0,2
lw $t1,-20($fp)
sw $t2,-260($fp)
mult $t0,$t1
mflo $t2
sw $t2,-264($fp)
lw $t0,-260($fp)
sw $t2,-264($fp)
addu $t2,$t0,$t2
sw $t2,-268($fp)
sw $t0,-260($fp)
li $t0,523
sw $t2,-268($fp)
subu $t2,$t2,$t0
sw $t2,-272($fp)
li $t0,53
sw $t2,-272($fp)
addu $t2,$t2,$t0
sw $t2,-276($fp)
li $t0,355
sw $t2,-276($fp)
addu $t2,$t2,$t0
sw $t2,-280($fp)
li $t0,2
sw $t1,-20($fp)
lw $t1,-24($fp)
sw $t2,-280($fp)
mult $t0,$t1
mflo $t2
sw $t2,-284($fp)
lw $t0,-280($fp)
sw $t2,-284($fp)
subu $t2,$t0,$t2
sw $t2,-288($fp)
sw $t2,-28($fp)
move $t8,$t2
sw $t0,-280($fp)
sw $t1,-24($fp)
sw $t2,-288($fp)
sw $t4,-228($fp)
sw $t6,-232($fp)
sw $t8,-28($fp)
sw $t9,-236($fp)
$label16:
$label10:
lw $t0,-24($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t1,-28($fp)
move $a0,$t1
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t2,-12($fp)
lw $t3,-8($fp)
bgt $t2,$t3,$label17
la $a0,string148
li $v0,4
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-24($fp)
sw $t1,-28($fp)
sw $t2,-12($fp)
sw $t3,-8($fp)
$label17:
lw $t0,-8($fp)
lw $t1,-12($fp)
bne $t0,$t1,$label18
la $a0,string151
li $v0,4
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-8($fp)
sw $t1,-12($fp)
$label18:
li $t0,0
sw $t0,-24($fp)
li $t1,0
sw $t1,-28($fp)
lw $t2,-8($fp)
lw $t3,-12($fp)
beq $t2,$t3,$label19
lw $t4,-28($fp)
li $t5,2
mult $t4,$t5
mflo $t6
sw $t6,-292($fp)
lw $t7,-24($fp)
subu $t8,$t7,$t6
sw $t8,-296($fp)
li $t9,0
bne $t8,$t9,$label21
sw $t2,-8($fp)
sw $t3,-12($fp)
sw $t4,-28($fp)
sw $t6,-292($fp)
sw $t7,-24($fp)
sw $t8,-296($fp)
$label22:
lw $t0,-24($fp)
li $t1,30
bge $t0,$t1,$label23
li $t2,2
div $t0,$t2
mflo $t3
sw $t3,-300($fp)
li $t4,2
mult $t3,$t4
mflo $t5
sw $t5,-304($fp)
subu $t6,$t5,$t0
sw $t6,-308($fp)
li $t7,-1
bne $t6,$t7,$label25
li $t8,1
addu $t9,$t0,$t8
sw $t9,-312($fp)
sw $t9,-24($fp)
move $t0,$t9
j $label24
sw $t0,-24($fp)
sw $t3,-300($fp)
sw $t5,-304($fp)
sw $t6,-308($fp)
sw $t9,-312($fp)
$label25:
lw $t0,-308($fp)
li $t1,0
bne $t0,$t1,$label26
lw $t2,-24($fp)
lw $t3,-28($fp)
addu $t4,$t2,$t3
sw $t4,-316($fp)
sw $t4,-24($fp)
move $t2,$t4
j $label24
sw $t0,-308($fp)
sw $t2,-24($fp)
sw $t3,-28($fp)
sw $t4,-316($fp)
$label26:
$label24:
lw $t0,-28($fp)
li $t1,1
addu $t2,$t0,$t1
sw $t2,-320($fp)
sw $t2,-28($fp)
move $t0,$t2
j $label22
sw $t0,-28($fp)
sw $t2,-320($fp)
$label23:
j $label20
$label21:
li $t0,0
li $t1,1
subu $t2,$t0,$t1
sw $t2,-324($fp)
sw $t2,-24($fp)
li $t3,0
li $t4,1
subu $t5,$t3,$t4
sw $t5,-328($fp)
sw $t5,-28($fp)
sw $t2,-324($fp)
sw $t5,-328($fp)
$label20:
$label19:
lw $t0,-24($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t1,-28($fp)
move $a0,$t1
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t2,-8($fp)
subu $t3,$t2,$t2
sw $t3,-332($fp)
beq $t3,0,$label27
la $a0,string191
li $v0,4
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-24($fp)
sw $t1,-28($fp)
sw $t2,-8($fp)
sw $t3,-332($fp)
$label27:
li $t0,0
li $t1,1
subu $t2,$t0,$t1
sw $t2,-336($fp)
sw $t2,int_global_var
li $t3,97
sw $t3,char_global_var
lw $ra,0($fp)
jr $ra
testSwitchCase:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,92
sw $a0,-8($fp)
sw $a1,-12($fp)
lw $t0,-8($fp)
li $t1,-1
bne $t0,$t1,$label29
sw $t0,-8($fp)
$label30:
lw $t0,-8($fp)
li $t1,20
mult $t0,$t1
mflo $t2
sw $t2,-16($fp)
li $t3,30
li $t4,5
div $t3,$t4
mflo $t5
sw $t5,-20($fp)
addu $t6,$t2,$t5
sw $t6,-24($fp)
li $t7,10
bge $t6,$t7,$label31
li $t8,0
bgt $t0,$t8,$label32
li $t9,1
sw $t2,-16($fp)
addu $t2,$t0,$t9
sw $t2,-28($fp)
sw $t2,-8($fp)
move $t0,$t2
sw $t0,-8($fp)
sw $t2,-28($fp)
sw $t5,-20($fp)
sw $t6,-24($fp)
$label32:
j $label30
$label31:
j $label28
$label29:
lw $t0,-8($fp)
li $t1,0
bne $t0,$t1,$label33
lw $s0,const_int_postive
sw $s0,-32($fp)
lw $s1,const_int_negtive
sw $s1,-36($fp)
lw $t2,-32($fp)
lw $t3,-36($fp)
mult $t2,$t3
mflo $t4
sw $t4,-40($fp)
subu $t5,$t0,$t4
sw $t5,-44($fp)
li $t6,0
bne $t5,$t6,$label34
sw $t0,-8($fp)
sw $t2,-32($fp)
sw $t3,-36($fp)
sw $t4,-40($fp)
sw $t5,-44($fp)
sw $s0,const_int_postive
sw $s1,const_int_negtive
$label35:
lw $t0,-8($fp)
li $t1,0
bgt $t0,$t1,$label36
li $t2,1
addu $t3,$t0,$t2
sw $t3,-48($fp)
li $t4,-2
mult $t3,$t4
mflo $t5
sw $t5,-52($fp)
sw $t5,-8($fp)
move $t0,$t5
j $label35
sw $t0,-8($fp)
sw $t3,-48($fp)
sw $t5,-52($fp)
$label36:
$label34:
j $label28
$label33:
lw $t0,-8($fp)
li $t1,1
bne $t0,$t1,$label37
li $t2,0
li $t3,233
subu $t4,$t2,$t3
sw $t4,-56($fp)
sw $t4,-8($fp)
move $t0,$t4
j $label28
sw $t0,-8($fp)
sw $t4,-56($fp)
$label37:
li $t0,233
sw $t0,-8($fp)
$label28:
lw $t0,-12($fp)
li $t1,97
bne $t0,$t1,$label39
lw $s0,const_char_a
sw $s0,-60($fp)
lw $t2,-60($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-60($fp)
sw $s0,const_char_a
$label39:
lw $t0,-12($fp)
li $t1,65
bne $t0,$t1,$label40
lw $s0,const_char_A
sw $s0,-64($fp)
lw $t2,-64($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-64($fp)
sw $s0,const_char_A
$label40:
lw $t0,-12($fp)
li $t1,57
bne $t0,$t1,$label41
lw $s0,const_char_num
sw $s0,-68($fp)
lw $t2,-68($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-68($fp)
sw $s0,const_char_num
$label41:
lw $t0,-12($fp)
li $t1,95
bne $t0,$t1,$label42
lw $s0,const_char_underline
sw $s0,-72($fp)
lw $t2,-72($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-72($fp)
sw $s0,const_char_underline
$label42:
lw $t0,-12($fp)
li $t1,43
bne $t0,$t1,$label43
lw $s0,const_char_add
sw $s0,-76($fp)
lw $t2,-76($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-76($fp)
sw $s0,const_char_add
$label43:
lw $t0,-12($fp)
li $t1,42
bne $t0,$t1,$label44
lw $s0,const_char_mul
sw $s0,-80($fp)
lw $t2,-80($fp)
sw $t2,-12($fp)
move $t0,$t2
j $label38
sw $t0,-12($fp)
sw $t2,-80($fp)
sw $s0,const_char_mul
$label44:
li $t0,119
sw $t0,-12($fp)
$label38:
li $t0,0
li $t1,0
bne $t0,$t1,$label46
j $label45
$label46:
$label45:
lw $t0,-8($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $t1,-12($fp)
move $a0,$t1
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
li $t2,0
li $t3,2
subu $t4,$t2,$t3
sw $t4,-84($fp)
sw $t4,int_global_var
li $t5,98
sw $t5,char_global_var
lw $ra,0($fp)
jr $ra
returnInt:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $t0,10
move $v0,$t0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnChar:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $t0,114
move $v0,$t0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnIntParameter:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,36
sw $a0,-8($fp)
sw $a1,-12($fp)
sw $a2,-16($fp)
sw $a3,-20($fp)
lw $t0,-8($fp)
li $t1,1
bne $t0,$t1,$label48
lw $t2,-12($fp)
sw $t2,-28($fp)
j $label47
sw $t0,-8($fp)
sw $t2,-12($fp)
$label48:
lw $t0,-8($fp)
li $t1,2
bne $t0,$t1,$label49
lw $t2,-16($fp)
sw $t2,-28($fp)
j $label47
sw $t0,-8($fp)
sw $t2,-16($fp)
$label49:
lw $t0,-8($fp)
li $t1,3
bne $t0,$t1,$label50
lw $t2,-20($fp)
sw $t2,-28($fp)
j $label47
sw $t0,-8($fp)
sw $t2,-20($fp)
$label50:
lw $t0,-8($fp)
li $t1,4
bne $t0,$t1,$label51
lw $t2,-24($fp)
sw $t2,-28($fp)
j $label47
sw $t0,-8($fp)
sw $t2,-24($fp)
$label51:
li $t0,0
sw $t0,-28($fp)
$label47:
lw $t0,-28($fp)
move $v0,$t0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
returnCharParameter:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
sw $a0,-8($fp)
sw $a1,-12($fp)
sw $a2,-16($fp)
sw $a3,-20($fp)
lw $t0,-8($fp)
li $t1,1
bne $t0,$t1,$label53
lw $t2,-12($fp)
move $v0,$t2
lw $ra,0($fp)
jr $ra
j $label52
sw $t0,-8($fp)
sw $t2,-12($fp)
$label53:
lw $t0,-8($fp)
li $t1,2
bne $t0,$t1,$label54
lw $t2,-16($fp)
move $v0,$t2
lw $ra,0($fp)
jr $ra
j $label52
sw $t0,-8($fp)
sw $t2,-16($fp)
$label54:
lw $t0,-8($fp)
li $t1,3
bne $t0,$t1,$label55
lw $t2,-20($fp)
move $v0,$t2
lw $ra,0($fp)
jr $ra
j $label52
sw $t0,-8($fp)
sw $t2,-20($fp)
$label55:
lw $t0,-8($fp)
li $t1,4
bne $t0,$t1,$label56
lw $t2,-24($fp)
move $v0,$t2
lw $ra,0($fp)
jr $ra
j $label52
sw $t0,-8($fp)
sw $t2,-24($fp)
$label56:
li $t0,48
move $v0,$t0
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
li $t0,10
sw $t0,-8($fp)
li $t0,0
li $t1,-1
subu $t2,$t0,$t1
sw $t2,-44($fp)
li $t3,0
li $t4,2
subu $t5,$t3,$t4
sw $t5,-48($fp)
sw $t2,-44($fp)
sw $t5,-48($fp)
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t5,-48($fp)
lw $t2,-44($fp)
sw $v0,-52($fp)
lw $s0,const_char_a
sw $s0,-56($fp)
lw $t6,-52($fp)
lw $t7,-56($fp)
subu $t8,$t6,$t7
sw $t8,-60($fp)
li $a0,2
move $a1,$t2
move $a2,$t5
lw $t9,-8($fp)
move $a3,$t9
sw $t8,-24($sp)
sw $t2,-44($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnIntParameter
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-44($fp)
lw $s0,const_char_a
sw $v0,-64($fp)
sw $s0,-68($fp)
li $t0,99
lw $t1,-68($fp)
sw $t2,-44($fp)
subu $t2,$t0,$t1
sw $t2,-72($fp)
lw $t0,-64($fp)
sw $t2,-72($fp)
mult $t0,$t2
mflo $t2
sw $t2,-76($fp)
sw $t0,-64($fp)
li $t0,2
sw $t2,-76($fp)
div $t2,$t0
mflo $t2
sw $t2,-80($fp)
li $t0,10
sw $t2,-80($fp)
addu $t2,$t0,$t2
sw $t2,-84($fp)
sw $t2,-12($fp)
lw $t0,-12($fp)
sw $t1,-68($fp)
li $t1,4
sw $t2,-84($fp)
div $t0,$t1
mflo $t2
sw $t2,-88($fp)
sw $s0,-92($fp)
sw $t0,-12($fp)
lw $t0,-92($fp)
sw $t2,-88($fp)
addu $t2,$t2,$t0
sw $t2,-96($fp)
sw $t0,-92($fp)
li $t0,97
sw $t2,-96($fp)
subu $t2,$t2,$t0
sw $t2,-100($fp)
li $t0,2
sw $t2,-100($fp)
subu $t2,$t2,$t0
sw $t2,-104($fp)
li $t0,0
li $t1,-2
sw $t2,-104($fp)
subu $t2,$t0,$t1
sw $t2,-108($fp)
lw $t0,-104($fp)
sll $v1,$t0,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
sw $t2,0($fp)
addu $fp,$fp,$v1
sw $t0,-104($fp)
lw $t0,-112($fp)
li $t1,0
sll $v1,$t1,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-112($fp)
sw $t0,-112($fp)
lw $t0,-12($fp)
lw $t1,-112($fp)
sw $t2,-108($fp)
div $t0,$t1
mflo $t2
sw $t2,-116($fp)
sw $t0,-12($fp)
li $t0,-2
sw $t2,-116($fp)
addu $t2,$t0,$t2
sw $t2,-120($fp)
sw $t1,-112($fp)
sw $t2,-120($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-120($fp)
lw $t1,-112($fp)
lw $s0,const_char_a
sw $v0,-124($fp)
lw $t0,-124($fp)
sw $t2,-120($fp)
addu $t2,$t2,$t0
sw $t2,-128($fp)
sw $t0,-124($fp)
li $t0,11
sw $t2,-128($fp)
subu $t2,$t2,$t0
sw $t2,-132($fp)
sw $t1,-112($fp)
sw $t2,-132($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-132($fp)
lw $t1,-112($fp)
lw $s0,const_char_a
sw $v0,-136($fp)
li $t0,0
sw $t1,-112($fp)
lw $t1,-136($fp)
sw $t2,-132($fp)
subu $t2,$t0,$t1
sw $t2,-140($fp)
sw $t1,-136($fp)
sw $t2,-140($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-140($fp)
lw $t1,-136($fp)
lw $s0,const_char_a
sw $v0,-144($fp)
sw $s0,-148($fp)
lw $t0,-144($fp)
sw $t1,-136($fp)
lw $t1,-148($fp)
sw $t2,-140($fp)
subu $t2,$t0,$t1
sw $t2,-152($fp)
sw $t2,-152($fp)
mult $t9,$t2
mflo $t2
sw $t2,-156($fp)
sw $t0,-144($fp)
li $t0,0
sw $t1,-148($fp)
li $t1,98
sw $t2,-156($fp)
subu $t2,$t0,$t1
sw $t2,-160($fp)
li $t0,99
sw $t2,-160($fp)
addu $t2,$t2,$t0
sw $t2,-164($fp)
sw $t2,-164($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnInt
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-164($fp)
lw $s0,const_char_a
sw $v0,-168($fp)
li $a0,4
move $a1,$t2
lw $t0,-168($fp)
move $a2,$t0
move $a3,$t9
sw $a0,0($sp)
li $a0,97
sw $a0,-24($sp)
lw $a0,0($sp)
sw $t0,-168($fp)
sw $t2,-164($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnIntParameter
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-164($fp)
lw $t0,-168($fp)
lw $s0,const_char_a
sw $v0,-172($fp)
sw $t0,-168($fp)
lw $t0,-156($fp)
lw $t1,-172($fp)
sw $t2,-164($fp)
addu $t2,$t0,$t1
sw $t2,-176($fp)
sw $t0,-156($fp)
lw $t0,-140($fp)
sw $t2,-176($fp)
addu $t2,$t0,$t2
sw $t2,-180($fp)
sw $t0,-140($fp)
lw $t0,-132($fp)
sll $v1,$t0,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
sw $t2,0($fp)
addu $fp,$fp,$v1
sw $t0,-132($fp)
li $t0,0
sw $t1,-172($fp)
li $t1,255
sw $t2,-180($fp)
subu $t2,$t0,$t1
sw $t2,-184($fp)
lw $t0,-188($fp)
li $t1,1
sll $v1,$t1,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-188($fp)
sw $t2,-184($fp)
addu $t2,$t2,$t0
sw $t2,-192($fp)
sw $t0,-188($fp)
li $t0,98
li $t1,2
sw $t2,-192($fp)
mult $t0,$t1
mflo $t2
sw $t2,-196($fp)
lw $t0,-192($fp)
sw $t2,-196($fp)
addu $t2,$t0,$t2
sw $t2,-200($fp)
sw $t0,-192($fp)
li $t0,196
sw $t2,-200($fp)
subu $t2,$t2,$t0
sw $t2,-204($fp)
li $t0,0
li $t1,98
sw $t2,-204($fp)
subu $t2,$t0,$t1
sw $t2,-208($fp)
li $t0,99
sw $t2,-208($fp)
addu $t2,$t2,$t0
sw $t2,-212($fp)
lw $t0,-204($fp)
sll $v1,$t0,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
sw $t2,0($fp)
addu $fp,$fp,$v1
sw $t0,-204($fp)
sw $t2,-212($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-212($fp)
lw $t0,-204($fp)
lw $s0,const_char_a
sw $v0,-216($fp)
sw $t0,-204($fp)
lw $t0,-216($fp)
sw $t0,-28($fp)
sw $t0,-216($fp)
sw $t2,-212($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-212($fp)
lw $t0,-216($fp)
lw $s0,const_char_a
sw $v0,-220($fp)
sw $t0,-216($fp)
sw $t2,-212($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnChar
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-212($fp)
lw $t0,-216($fp)
lw $s0,const_char_a
sw $v0,-224($fp)
sw $s0,-228($fp)
li $a0,4
li $a1,97
sw $t0,-216($fp)
lw $t0,-220($fp)
move $a2,$t0
sw $t0,-220($fp)
lw $t0,-224($fp)
move $a3,$t0
sw $t0,-224($fp)
lw $t0,-228($fp)
sw $t0,-24($sp)
sw $t0,-228($fp)
sw $t2,-212($fp)
sw $t5,-48($fp)
sw $t6,-52($fp)
sw $t7,-56($fp)
sw $t8,-60($fp)
sw $t9,-8($fp)
sw $s0,const_char_a
jal returnCharParameter
move $sp,$fp
lw $fp,-4($fp)
lw $t9,-8($fp)
lw $t8,-60($fp)
lw $t7,-56($fp)
lw $t6,-52($fp)
lw $t5,-48($fp)
lw $t2,-212($fp)
lw $t0,-228($fp)
lw $s0,const_char_a
sw $v0,-232($fp)
sw $t0,-228($fp)
li $t0,0
lw $t1,-232($fp)
sll $v1,$t0,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
sw $t1,0($fp)
addu $fp,$fp,$v1
lw $t0,-236($fp)
sw $t1,-232($fp)
li $t1,0
sll $v1,$t1,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-236($fp)
sw $t0,-236($fp)
li $t0,1
lw $t1,-236($fp)
sll $v1,$t0,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
sw $t1,0($fp)
addu $fp,$fp,$v1
li $t0,2
sw $t1,-236($fp)
li $t1,120
sll $v1,$t0,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
sw $t1,0($fp)
addu $fp,$fp,$v1
lw $t0,-12($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-12($fp)
lw $t0,-240($fp)
li $t1,0
sll $v1,$t1,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-240($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-240($fp)
lw $t0,-244($fp)
li $t1,1
sll $v1,$t1,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-244($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-244($fp)
lw $t0,-248($fp)
li $t1,2
sll $v1,$t1,2
addiu $v1,$v1,16
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-248($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-248($fp)
lw $t0,-28($fp)
move $a0,$t0
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-28($fp)
lw $t0,-252($fp)
li $t1,0
sll $v1,$t1,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-252($fp)
move $a0,$t0
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-252($fp)
lw $t0,-256($fp)
li $t1,1
sll $v1,$t1,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-256($fp)
move $a0,$t0
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-256($fp)
lw $t0,-260($fp)
li $t1,2
sll $v1,$t1,2
addiu $v1,$v1,32
subu $fp,$fp,$v1
lw $t0,0($fp)
addu $fp,$fp,$v1
sw $t0,-260($fp)
move $a0,$t0
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
sw $t0,-260($fp)
li $t0,0
li $t1,3
sw $t2,-212($fp)
subu $t2,$t0,$t1
sw $t2,-264($fp)
sw $t2,int_global_var
li $t0,99
sw $t0,char_global_var
lw $ra,0($fp)
jr $ra
testInputOutput:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
li $v0,5
syscall
move $t0,$v0
sw $t0,-8($fp)
li $v0,12
syscall
move $t1,$v0
sw $t1,-12($fp)
la $a0,string451
li $v0,4
syscall
la $a0,string621
li $v0,4
syscall
la $a0,string452
li $v0,4
syscall
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
la $a0,string453
li $v0,4
syscall
move $a0,$t1
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
li $t2,0
li $t3,4
subu $t4,$t2,$t3
sw $t4,-24($fp)
sw $t4,-16($fp)
li $t5,100
sw $t5,-20($fp)
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
sw $a0,-8($fp)
sw $a1,-12($fp)
sw $a2,-16($fp)
sw $a3,-20($fp)
li $t0,0
sw $t0,-24($fp)
lw $t1,-8($fp)
lw $t2,-12($fp)
bge $t1,$t2,$label57
lw $t3,-16($fp)
lw $t4,-20($fp)
blt $t3,$t4,$label58
li $t5,1
sw $t5,-24($fp)
sw $t1,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
$label58:
$label57:
lw $t0,-24($fp)
move $v0,$t0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
judge2:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,32
sw $a0,-8($fp)
sw $a1,-12($fp)
sw $a2,-16($fp)
sw $a3,-20($fp)
li $t0,0
sw $t0,-24($fp)
lw $t1,-8($fp)
lw $t2,-12($fp)
bge $t1,$t2,$label59
lw $t3,-16($fp)
lw $t4,-20($fp)
bgt $t3,$t4,$label60
li $t5,1
sw $t5,-24($fp)
sw $t1,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
$label60:
$label59:
lw $t0,-24($fp)
move $v0,$t0
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
Qsort:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,76
sw $a0,-8($fp)
sw $a1,-12($fp)
lw $t0,-8($fp)
sw $t0,-16($fp)
lw $t1,-12($fp)
sw $t1,-20($fp)
lw $t2,-28($fp)
lw $t3,-16($fp)
sll $v1,$t3,2
lw $t2,sort($v1)
sw $t2,-28($fp)
sw $t2,-24($fp)
blt $t0,$t1,$label61
lw $ra,0($fp)
jr $ra
sw $t0,-8($fp)
sw $t1,-12($fp)
sw $t2,-28($fp)
sw $t3,-16($fp)
$label61:
$label62:
lw $t0,-16($fp)
lw $t1,-20($fp)
bge $t0,$t1,$label63
sw $t0,-16($fp)
sw $t1,-20($fp)
$label64:
lw $t0,-32($fp)
lw $t1,-20($fp)
sll $v1,$t1,2
lw $t0,sort($v1)
sw $t0,-32($fp)
lw $t2,-16($fp)
move $a0,$t2
move $a1,$t1
move $a2,$t0
lw $t3,-24($fp)
move $a3,$t3
sw $t0,-32($fp)
sw $t1,-20($fp)
sw $t2,-16($fp)
sw $t3,-24($fp)
jal judge1
move $sp,$fp
lw $fp,-4($fp)
lw $t3,-24($fp)
lw $t2,-16($fp)
lw $t1,-20($fp)
lw $t0,-32($fp)
sw $v0,-36($fp)
lw $t4,-36($fp)
li $t5,1
bne $t4,$t5,$label65
li $t6,1
subu $t7,$t1,$t6
sw $t7,-40($fp)
sw $t7,-20($fp)
move $t1,$t7
j $label64
sw $t0,-32($fp)
sw $t1,-20($fp)
sw $t2,-16($fp)
sw $t3,-24($fp)
sw $t4,-36($fp)
sw $t7,-40($fp)
$label65:
lw $t0,-44($fp)
lw $t1,-20($fp)
sll $v1,$t1,2
lw $t0,sort($v1)
sw $t0,-44($fp)
lw $t2,-16($fp)
sll $v1,$t2,2
sw $t0,sort($v1)
sw $t0,-44($fp)
sw $t1,-20($fp)
sw $t2,-16($fp)
$label66:
lw $t0,-48($fp)
lw $t1,-16($fp)
sll $v1,$t1,2
lw $t0,sort($v1)
sw $t0,-48($fp)
move $a0,$t1
lw $t2,-20($fp)
move $a1,$t2
move $a2,$t0
lw $t3,-24($fp)
move $a3,$t3
sw $t0,-48($fp)
sw $t1,-16($fp)
sw $t2,-20($fp)
sw $t3,-24($fp)
jal judge2
move $sp,$fp
lw $fp,-4($fp)
lw $t3,-24($fp)
lw $t2,-20($fp)
lw $t1,-16($fp)
lw $t0,-48($fp)
sw $v0,-52($fp)
lw $t4,-52($fp)
li $t5,1
bne $t4,$t5,$label67
li $t6,1
addu $t7,$t1,$t6
sw $t7,-56($fp)
sw $t7,-16($fp)
move $t1,$t7
j $label66
sw $t0,-48($fp)
sw $t1,-16($fp)
sw $t2,-20($fp)
sw $t3,-24($fp)
sw $t4,-52($fp)
sw $t7,-56($fp)
$label67:
lw $t0,-60($fp)
lw $t1,-16($fp)
sll $v1,$t1,2
lw $t0,sort($v1)
sw $t0,-60($fp)
lw $t2,-20($fp)
sll $v1,$t2,2
sw $t0,sort($v1)
j $label62
sw $t0,-60($fp)
sw $t1,-16($fp)
sw $t2,-20($fp)
$label63:
lw $t0,-16($fp)
lw $t1,-24($fp)
sll $v1,$t0,2
sw $t1,sort($v1)
li $t2,1
subu $t3,$t0,$t2
sw $t3,-64($fp)
lw $t4,-8($fp)
move $a0,$t4
move $a1,$t3
sw $t0,-16($fp)
sw $t1,-24($fp)
sw $t3,-64($fp)
sw $t4,-8($fp)
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
lw $t4,-8($fp)
lw $t3,-64($fp)
lw $t1,-24($fp)
lw $t0,-16($fp)
li $t5,1
addu $t6,$t0,$t5
sw $t6,-68($fp)
move $a0,$t6
lw $t7,-12($fp)
move $a1,$t7
sw $t0,-16($fp)
sw $t1,-24($fp)
sw $t3,-64($fp)
sw $t4,-8($fp)
sw $t6,-68($fp)
sw $t7,-12($fp)
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
lw $t7,-12($fp)
lw $t6,-68($fp)
lw $t4,-8($fp)
lw $t3,-64($fp)
lw $t1,-24($fp)
lw $t0,-16($fp)
lw $ra,0($fp)
jr $ra
lw $ra,0($fp)
jr $ra
testRecursion:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,28
li $t0,0
li $t1,5
sll $v1,$t0,2
sw $t1,sort($v1)
li $t2,1
li $t3,8
sll $v1,$t2,2
sw $t3,sort($v1)
li $t4,2
li $t5,3
sll $v1,$t4,2
sw $t5,sort($v1)
li $t6,3
li $t7,6
sll $v1,$t6,2
sw $t7,sort($v1)
li $t8,4
li $t9,4
sll $v1,$t8,2
sw $t9,sort($v1)
li $t0,5
li $t1,1
sll $v1,$t0,2
sw $t1,sort($v1)
li $t0,6
li $t1,2
sll $v1,$t0,2
sw $t1,sort($v1)
li $t0,7
li $t1,7
sll $v1,$t0,2
sw $t1,sort($v1)
li $a0,0
li $a1,7
jal Qsort
move $sp,$fp
lw $fp,-4($fp)
li $t0,0
sw $t0,-8($fp)
$label68:
lw $t0,-8($fp)
li $t1,8
bge $t0,$t1,$label69
lw $t2,-12($fp)
sll $v1,$t0,2
lw $t2,sort($v1)
sw $t2,-12($fp)
move $a0,$t2
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
li $t3,1
addu $t4,$t0,$t3
sw $t4,-16($fp)
sw $t4,-8($fp)
move $t0,$t4
j $label68
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t4,-16($fp)
$label69:
li $t0,0
li $t1,5
subu $t2,$t0,$t1
sw $t2,-20($fp)
sw $t2,int_global_var
li $t3,101
sw $t3,char_global_var
lw $ra,0($fp)
jr $ra
testCallFunction:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,36
li $v0,5
syscall
move $t0,$v0
sw $t0,-8($fp)
li $t1,0
bne $t0,$t1,$label71
sw $t0,-8($fp)
jal testIfWhile
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-8($fp)
j $label70
sw $t0,-8($fp)
$label71:
lw $t0,-8($fp)
li $t1,1
bne $t0,$t1,$label72
lw $s0,const_int_postive
sw $s0,-12($fp)
lw $s1,const_char_a
sw $s1,-16($fp)
lw $t2,-12($fp)
move $a0,$t2
lw $t3,-16($fp)
move $a1,$t3
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $s0,const_int_postive
sw $s1,const_char_a
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
lw $t3,-16($fp)
lw $t2,-12($fp)
lw $t0,-8($fp)
lw $s1,const_char_a
lw $s0,const_int_postive
lw $s2,const_int_negtive
sw $s2,-20($fp)
lw $t4,-20($fp)
move $a0,$t4
li $a1,43
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
sw $s0,const_int_postive
sw $s1,const_char_a
sw $s2,const_int_negtive
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
lw $t4,-20($fp)
lw $t3,-16($fp)
lw $t2,-12($fp)
lw $t0,-8($fp)
lw $s2,const_int_negtive
lw $s1,const_char_a
lw $s0,const_int_postive
li $a0,0
li $a1,95
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
sw $s0,const_int_postive
sw $s1,const_char_a
sw $s2,const_int_negtive
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
lw $t4,-20($fp)
lw $t3,-16($fp)
lw $t2,-12($fp)
lw $t0,-8($fp)
lw $s2,const_int_negtive
lw $s1,const_char_a
lw $s0,const_int_postive
li $a0,2
li $a1,57
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
sw $s0,const_int_postive
sw $s1,const_char_a
sw $s2,const_int_negtive
jal testSwitchCase
move $sp,$fp
lw $fp,-4($fp)
lw $t4,-20($fp)
lw $t3,-16($fp)
lw $t2,-12($fp)
lw $t0,-8($fp)
lw $s2,const_int_negtive
lw $s1,const_char_a
lw $s0,const_int_postive
j $label70
sw $t0,-8($fp)
sw $t2,-12($fp)
sw $t3,-16($fp)
sw $t4,-20($fp)
sw $s0,const_int_postive
sw $s1,const_char_a
sw $s2,const_int_negtive
$label72:
lw $t0,-8($fp)
li $t1,2
bne $t0,$t1,$label73
sw $t0,-8($fp)
jal testAssign
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-8($fp)
j $label70
sw $t0,-8($fp)
$label73:
lw $t0,-8($fp)
li $t1,3
bne $t0,$t1,$label74
sw $t0,-8($fp)
jal testInputOutput
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-8($fp)
j $label70
sw $t0,-8($fp)
$label74:
lw $t0,-8($fp)
li $t1,4
bne $t0,$t1,$label75
sw $t0,-8($fp)
jal testRecursion
move $sp,$fp
lw $fp,-4($fp)
lw $t0,-8($fp)
j $label70
sw $t0,-8($fp)
$label75:
$label70:
lw $s0,int_global_var
sw $s0,-24($fp)
lw $t0,-24($fp)
move $a0,$t0
li $v0,1
syscall
la $a0,string621
li $v0,4
syscall
lw $s1,char_global_var
sw $s1,-28($fp)
lw $t1,-28($fp)
move $a0,$t1
li $v0,11
syscall
la $a0,string621
li $v0,4
syscall
lw $ra,0($fp)
jr $ra
main:
sw $fp,-4($sp)
move $fp,$sp
sw $ra,0($fp)
subiu $sp,$sp,12
li $t0,0
sw $t0,int_global_var
li $t1,95
sw $t1,char_global_var
jal testCallFunction
move $sp,$fp
lw $fp,-4($fp)
jal testTrick
move $sp,$fp
lw $fp,-4($fp)