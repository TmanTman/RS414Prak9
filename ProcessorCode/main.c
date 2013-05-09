register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");

char grid[10000] = {5};
char rowOutput[10000] = {3};
int tap[3] = {2, 4, 3};

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
	ldr r11, =__cpp(&rowOutput);
	//Outer Loop
newRow
	//Reset column counter
	mov r1, #0;
	//Check whether last row has been reached
	cmp r10, #100; 
	beq gridDone;
	//Inner Loop
colStart ; Inner loop
	//Check whether last column has been reached
	cmp r1, #100;
	beq rowDone;
	//Load row values
	cmp r1, #0; //special load operation at start of row
	beq counter0 ;
	cmp r1, #1; //special load operation at start of row
	beq counter1 ;
	b normalLoad ;
counter0	
	mov r2, #0;
	mov r3, #0;
	ldrb r4, [r12];
	b next ;
counter1
	mov r2, #0;
	ldrb r3, [r12];
	ldrb r4, [r12,#1];
	b next ;
normalLoad	//normal load operation
	sub r1, #2; To reach first address, as R1 points to leading address
	ldrb r2, [r12, r1];
	add r1, #1;
	ldrb r3, [r12, r1];
	add r1, #1;
	ldrb r4, [r12, r1];
next
	//Do multiplication
	mul r4, r7;
	mul r3, r6;
	mul r2, r5;
	//add all taps
	add r2, r3;
	add r2, r4;
	//move result to memory
	strb r2, [r11, r1]
	//loop with counter r1
	add r1, #1;
	b colStart
	//return to main
rowDone
	add r10, #1; increase row counter
	add r12, #100; shift r12 to beginning of next row
	add r11, #100; shift r11 to beginning of next row
	b newRow
gridDone	
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

int main (void) 
{
	initGrid();
	firRow();
}
