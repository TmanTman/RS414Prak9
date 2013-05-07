register unsigned int _R0 __asm("r0");  // method 2 : declare 'C' names for the registers
register unsigned int _R1 __asm("r1");
int x;
__asm int adder (int a, int b)
{
  ldr r3, =__cpp(&x);	 	// method 3. Use the 'C' variables in assembly (fetch the address of x)
	ldr r0, [r3];        	// related to method 3 - fetch the value of x
	add R0, R1;
}

int main (void)
{
	int result, a, b;
	x=3;
	_R0=1;			// related to method 2. This will change the register values directly
	_R1=2;
	result==adder(_R0,_R1);
	__asm("ADD a, b");       //method 1 : Here you can use the 'C' variables directly
}

