//init with bogus values to ensure memory area has write permissions
char grid[10000] = {5};
char rowOutput[10000] = {3};
char colOutput[10000] = {3};
int tap[3] = {2, 4, 3};
int outputBase = 0;

__asm void firRow(void)
{
	//Initialize
	;load output array address in memory
	ldr r12, =__cpp(&outputBase)
	ldr r11, =__cpp(&rowOutput)
	str r11, [r12]
	;init counters
	mov r1, #0; column counter
	mov r0, #0; row counter
	;Load gain values
 	ldr r12, =__cpp(&tap);
 	ldr r11, [r12];
 	ldr r10, [r12,#4];
	ldr r9, [r12,#8];
	ldr r12, =__cpp(&grid); pointer to grid start address
	//Outer Loop
newRow
	//Reset column counter
	mov r1, #0;
	//Check whether last row has been reached
	cmp r0, #100; 
	beq rgridDone;
	//Inner Loop
colStart 
	//Check whether last column has been reached
	cmp r1, #100;
	beq rowDone;
	//Load row values
	cmp r1, #0; special load operation at start of row
	beq rcounter0 ;
	cmp r1, #1; special load operation at start of row
	beq rcounter1 ;
	b rnormalLoad ;normal load operation
rcounter0	
	mov r4, #0;
	mov r5, #0;
	ldrb r6, [r12];
	b rnext ;
rcounter1
	mov r4, #0;
	ldrb r5, [r12];
	ldrb r6, [r12,#1];
	b rnext ;
rnormalLoad	
	sub r1, #2; To reach first address, as R1 points to leading address
	ldrb r4, [r12, r1];
	add r1, #1;
	ldrb r5, [r12, r1];
	add r1, #1;
	ldrb r6, [r12, r1];
rnext
	//Do multiplication
	mul r6, r11;
	mul r5, r10;
	mul r4, r9;
	//add all taps
	add r4, r5;
	add r4, r6;
	//move result to memory
		push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	strb r4, [r11, r1]
	pop {r11, r12}
	//loop with counter r1
	add r1, #1;
	b colStart
rowDone
	add r0, #1; increase row counter
	add r12, #100; shift r12 to beginning of next row
	;shift pointer to output grid with 100
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	add r11, #100;r11 now holds value 100 places further that base address
	str r11, [r12]
	pop {r11, r12}
	b newRow
rgridDone	
	bx lr;
}

__asm void firColumn(void)
{
	//Initialize
	;init the output array base value to memory
	ldr r12, =__cpp(&outputBase)
	ldr r11, =__cpp(&colOutput)
	str r11, [r12]
	;init counters
	mov r1, #0; column counter
	mov r0, #0; row counter
	;Load gain values
 	ldr r12, =__cpp(&tap);
 	ldr r11, [r12];
 	ldr r10, [r12,#4];
	ldr r9, [r12,#8];
	ldr r12, =__cpp(&grid); pointer to grid start address
	//Outer loop
newColumn
	//Reset row counter
	mov r0, #0;
	//Check whether last column has been reached
	cmp r1, #100; 
	beq cgridDone;
	//Inner Loop
rowLoop 
	//Check whether last row has been reached
	cmp r0, #100;
	beq colDone;
	//Load column values
	cmp r0, #0; //special load operation at start of row
	beq ccounter0 ;
	cmp r0, #1; //special load operation at start of row
	beq ccounter1 ;
	b cnormalLoad ;
ccounter0	
	mov r4, #0;
	mov r5, #0;
	ldrb r6, [r12];
	b cnext ;
ccounter1
	mov r4, #0;
	sub r12, #100
	ldrb r5, [r12];
	add r12, #100
	ldrb r6, [r12];
	b cnext ;
cnormalLoad	//normal load operation
	sub r12, #200; To reach first address, as r12 points to leading address
	ldrb r4, [r12];
	add r12, #100;
	ldrb r5, [r12]
	add r12, #100;
	ldrb r6, [r12]
cnext
	//Do multiplication
	mul r6, r11;
	mul r5, r10;
	mul r4, r9;
	//add all taps
	add r4, r5;
	add r4, r6;
	//move result to memory
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	strb r4, [r11]
	pop {r11, r12}
	//increase column counter and base addresses
	add r0, #1
	add r12, #100
	;increase base address of output grid
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	add r11, #100;r11 now holds value 100 places further that base address
	str r11, [r12]
	pop {r11, r12}
	b rowLoop
colDone
	add r1, #1; increase row counter
	//restore r12 to base of column
	sub r12, #4000
	sub r12, #4000
	sub r12, #2000
	add r12, #1; shift r12 to base of next column
	//restore r11 to base of column
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	sub r11, #4000
	sub r11, #4000
	sub r11, #2000
	add r11, #1; shift r11 to beginning of next column 
	str r11, [r12]; store r11 to memory
	pop {r11, r12}
	b newColumn
cgridDone	
	bx lr;
}

void initGrid(void)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			if (i >= 40 && i <= 59 && j >= 40 && j <= 59) grid[i*100+j] = 70;
			else grid[i*100+j] = 5;
		}
	}
}

__asm void testOB(void)
{
	//R12 will hold the pointer to the outputBase integer
	//During init, R11 holds the pointer to the outputArray which will be loaded into outputBase
	//Initialize
	push {r11, r12};Hierdie is nie nodig tydens init want ons sal later r11 en r12 se regte waardes inisialiseer
	ldr r12, =__cpp(&outputBase)
	ldr r11, =__cpp(&colOutput)
	str r11, [r12]
	pop {r11, r12}
	//Init done: the int outputBase now contains the start address to the output grid
	//To write to the output grid (value in r4 to be stored in output array):
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	strb r4, [r11]
	pop {r11, r12}
	//To increment the base value from where we will be operating:
	//r12 will hold the address of outputBase integer
	//r11 will hold the address of the outputgrid, which is the value inside outputBase integer
	push {r11, r12}
	ldr r12, =__cpp(&outputBase)
	ldr r11, [r12]
	add r11, #100;r11 now holds value 100 places further that base address
	str r11, [r12]
	pop {r11, r12}
}

int main (void) 
{
	initGrid();
	firRow();
	firColumn();
}
