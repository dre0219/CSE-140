# Traditional Matrix Multiply program
		.data
matrix_a:
		.word   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
		.word  13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
		.word  25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
		.word  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
		.word  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
		.word  61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72
		.word  73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84
		.word  85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96
		.word  97, 98, 99,100,101,102,103,104,105,106,107,108
		.word 109,110,111,112,113,114,115,116,117,118,119,120
		.word 121,122,123,124,125,126,127,128,129,130,131,132
		.word 133,134,135,136,137,138,139,140,141,142,143,144

matrix_b:
		.word 133,134,135,136,137,138,139,140,141,142,143,144
		.word 121,122,123,124,125,126,127,128,129,130,131,132
		.word 109,110,111,112,113,114,115,116,117,118,119,120
		.word  97, 98, 99,100,101,102,103,104,105,106,107,108
		.word  85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96
		.word  73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84
		.word  61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72
		.word  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
		.word  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
		.word  25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
		.word  13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
		.word   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12

matrix_c:
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		.word   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0

n:		.word 12

nline:  .asciiz "\n"				#Define new line string
sp:		.asciiz " "
msga: 	.asciiz "Matrix A is: \n"
msgb: 	.asciiz "Matrix B is: \n"
msgc: 	.asciiz "Matrix C=A*B is: \n"

		.text
		.globl main
main:

		la	$s1, n
		lw	$s1, 0($s1)
		la	$s2, matrix_a
		la	$s3, matrix_b
		la	$s4, matrix_c

		la	$a0, msga
		la 	$a1, matrix_a
		jal	PRINT_MAT 
		la	$a0, msgb
		la 	$a1, matrix_b
		jal	PRINT_MAT 

# Your CODE HERE
#initializing a, b, and c to 0
li $t0, 0 #a
li $t1, 0 #b
li $t2, 0 #c
		
		#loading the element size into $t8  and row size into $t9
		li $t8, 4 #Every element is 4 bytes large
		li $t9, 12 #Every row has 12 elements 
		
		#The 1st a loop to iterate over rows
		AFOR:
		beq $t0, $s1, ADONE	
		#Here we have the second nested b loop to iterate over the columns
		BFOR:
		beq $t1, $s1, BDONE 
		#We lastly have the third nested c loop to perform the multiplication
		CFOR:
		beq $t2, $s1, CDONE 
				
				
				
				
				
				
				#calculate current element address for a[a][c] and store into $s5
				mult $t9, $t0 #multiply it row_size * a
				mflo $t7
				add $t3, $t2, $t7 #add c
				mult $t3, $t8 
				mflo $t3
				add $t3, $s2, $t3 
				lw $s5, 0($t3)
				#calculate current element address for b[c][b] and store into $s6
				mult $t9, $t2 #multiply it row_size * c
				mflo $t7
				add $t4, $t1, $t7 #add b
				mult $t4, $t8 
				mflo $t4
				add $t4, $s3, $t4 				
				lw $s6, 0($t4)
				#calculate current element address for c[a][b] and store into $s7
				mult $t9, $t0 #multiply it row_size * a
				mflo $t7
				add $t5, $t1, $t7 #add b
				mult $t5, $t8
				mflo $t5
				add $t5, $s4, $t5  				
				lw $s7, 0($t5)
				#calculate value to store into c[a][b]
				mult $s5, $s6
				mflo $t7
				add $t6, $t7, $s7
				sw $t6, 0($t5)
				addi $t2, $t2, 1 #inputing c
				b CFOR
				
				
				
				
				
				
		CDONE:		
		li $t2, 0 #reseting c back to 0
		addi $t1, $t1, 1 #increment b
		b BFOR

		BDONE:
		li $t1, 0 #reseting b back to 0
		addi $t0, $t0, 1  #increment a
		b AFOR
			
		ADONE:	
# End CODE

		la	$a0, msgc
		la 	$a1, matrix_c
		jal	PRINT_MAT 

#   Exit
		li	 $v0,10
    	syscall


PRINT_MAT:	li	$v0,4
		syscall
		addi $a2,$0,0	
PL4:	bge	$a2,$s1,PL1
		addi $a3,$0,0
PL3:	bge	$a3,$s1,PL2

		lw	$a0,0($a1)
		li	$v0,1
		syscall
		la	$a0,sp
		li	$v0,4
		syscall
		addi $a1,$a1,4
		addi $a3,$a3,1
		b 	PL3

PL2:	addi	$a2,$a2,1
		la	$a0,nline
		li	$v0,4
		syscall
		b	PL4
PL1:	jr	$ra
