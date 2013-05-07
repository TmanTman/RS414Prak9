register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");

char grid[16] = {5};

__asm int memLocate(void) 
{
  ldr r3, =__cpp(&grid);	 	// method 3. Use the 'C' variables in assembly (fetch the address of x)
	ldr r0, [r3];        	// related to method 3 - fetch the value of x
};

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
	_R0 = 1;
	_R1 = 2;
	initGrid();
	memLocate();
	
}
