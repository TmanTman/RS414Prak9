register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");

char grid[16] = {5};
int tap[2] = {2, 4};

__asm void memLocate(void) 
{
  ldr r3, =__cpp(&grid);	 	// method 3. Use the 'C' variables in assembly (fetch the address of x)
	ldr r0, [r3];        	// related to method 3 - fetch the value of x
	ldr r4, =__cpp(&tap);
	ldr r1, [r4];
	bx lr
}

__asm void firColumn(void)
{
 	//Load constant values
 	ldr r1, =__cpp(&tap);
 	ldr r6, [r1];
 	ldr r7, [r1,#4];k
	//Load cloumn values
	ldr r1, =__cpp(&grid);
	ldrb r2, [r1]
	ldrb r3, [r1, #1];
	//Do multiplication and add all taps
	mul r2, r6;
	mul r3, r7;
	add r2, r3;
	//move result to memory
	str r2, [r1]
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

void cFunc(void)
{
	grid[0] = 10;
}

int main (void) 
{
	_R0 = 1;
	_R1 = 2;
	cFunc();
	initGrid();
	memLocate();
	cFunc();
	firColumn();
}