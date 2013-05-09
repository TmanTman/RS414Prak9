register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");

char grid[10000] = {5};
char rowOutput[10000] = {3};
char colOutput[10000] = {3};
int tap[3] = {2, 4, 3};
int outputBase = 0;

__asm void firRow(void)
{
	//Initialize 
	mov r1, #0; column counter
	mov r10, #0; row counter
	;Load gain values
 	ldr r12, =__cpp(&tap);
 	ldr r7, [r12];
 	ldr r6, [r12,#4];
	ldr r5, [r12,#8];
	ldr r12, =__cpp(&grid); pointer to grid start address
	//Outer Loop
newRow
	//Reset column counter
	mov r1, #0;
	//Check whether last row has been reached
	cmp r10, #100; 
	beq rgridDone;
	//Inner Loop
colStart 
	//Check whether last column has been reached
	cmp r1, #100;
	beq rowDone;
	//Load row values
	cmp r1, #0; //special load operation at start of row
	beq rcounter0 ;
	cmp r1, #1; //special load operation at start of row
	beq rcounter1 ;
	b rnormalLoad ;
rcounter0	
	mov r2, #0;
	mov r3, #0;
	ldrb r4, [r12];
	b rnext ;
rcounter1
	mov r2, #0;
	ldrb r3, [r12];
	ldrb r4, [r12,#1];
	b rnext ;
rnormalLoad	//normal load operation
	sub r1, #2; To reach first address, as R1 points to leading address
	ldrb r2, [r12, r1];
	add r1, #1;
	ldrb r3, [r12, r1];
	add r1, #1;
	ldrb r4, [r12, r1];
rnext
	//Do multiplication
	mul r4, r7;
	mul r3, r6;
	mul r2, r5;
	//add all taps
	add r2, r3;
	add r2, r4;
	//move result to memory
	push {r11, r12}
	ldr r11, =__cpp(&outputBase)
	ldr r12, =__cpp(&rowOutput)
	str r12, [r11, r1]
	
	strb r2, [r11, r1]
	//loop with counter r1
	add r1, #1;
	b colStart
rowDone
	add r10, #1; increase row counter
	add r12, #100; shift r12 to beginning of next row
	add r11, #100; shift r11 to beginning of next row
	b newRow
rgridDone	
	bx lr;
}

__asm void firColumn(void)
{
	//Initialize 
	mov r1, #0; column counter
	mov r10, #0; row counter
	;Load gain values
 	ldr r12, =__cpp(&tap);
 	ldr r7, [r12];
 	ldr r6, [r12,#4];
	ldr r5, [r12,#8];
	ldr r12, =__cpp(&grid); pointer to grid start address
	ldr r11, =__cpp(&colOutput);
	//Outer loop
newColumn
	//Reset row counter
	mov r10, #0;
	//Check whether last column has been reached
	cmp r1, #100; 
	beq cgridDone;
	//Inner Loop
rowLoop 
	//Check whether last row has been reached
	cmp r10, #100;
	beq colDone;
	//Load column values
	cmp r10, #0; //special load operation at start of row
	beq ccounter0 ;
	cmp r10, #1; //special load operation at start of row
	beq ccounter1 ;
	b cnormalLoad ;
ccounter0	
	mov r2, #0;
	mov r3, #0;
	ldrb r4, [r12];
	b cnext ;
ccounter1
	mov r2, #0;
	sub r12, #100
	ldrb r3, [r12];
	add r12, #100
	ldrb r4, [r12];
	b cnext ;
cnormalLoad	//normal load operation
	sub r12, #200; To reach first address, as R10 points to leading address
	ldrb r2, [r12];
	add r12, #100;
	ldrb r3, [r12]
	add r12, #100;
	ldrb r4, [r12]
cnext
	//Do multiplication
	mul r4, r7;
	mul r3, r6;
	mul r2, r5;
	//add all taps
	add r2, r3;
	add r2, r4;
	//move result to memory
	strb r2, [r11]
	//increase column counter and base addresses
	add r10, #1
	add r12, #100
	add r11, #100
	b rowLoop
colDone
	add r1, #1; increase row counter
	//restore r12 to base of column
	sub r12, #4000
	sub r12, #4000
	sub r12, #2000
	add r12, #1; shift r12 to base of next column
	//restore r11 to base of column
	sub r11, #4000
	sub r11, #4000
	sub r11, #2000
	add r11, #1; shift r11 to beginning of next column
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
	//To write to the output grid (value in r2 to be stored in output array):
	push {r12}
	ldr r12, =__cpp(&outputBase)
	strb r2, [r12]
	pop {r12}
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
	testOB();
}
