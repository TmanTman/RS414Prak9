register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");

char grid[16] = {5};
int tap[3] = {2, 4, 3};

__asm void firColumn(void)
{
	//Initialize counter
	mov r1, #0;
	//Load constant values
 	ldr r12, =__cpp(&tap);
 	ldr r7, [r12];
 	ldr r6, [r12,#4];
	ldr r5, [r12,#8];
	//Load row values
	ldr r12, =__cpp(&grid); //pointer to grid start address
	//loading values from grid
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
	ldrb r2, [r12]
	ldrb r3, [r12, #1];
	ldrb r4, [r12, #2];
next
	//Do multiplication
	mul r4, r7;
	mul r3, r6;
	mul r2, r5;
	//add all taps
	add r2, r3;
	add r2, r4;
	//move result to memory
	strb r2, [r12]
	//return to main
	bx lr;
}

void initGrid(void)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (i >= 1 && i <= 2 && j >= 1 && j <= 2) grid[i*4+j] = 70;
			else grid[i*4+j] = 5;
		}
	}
}

int main (void) 
{
	initGrid();
	firColumn();
}
