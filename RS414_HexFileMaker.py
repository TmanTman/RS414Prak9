#print("Hello", "World")
import sys
for x in range(100):
 for y in range(100):
  if 39 < y < 60 and 39 < x < 60:
   sys.stdout.write("07")
   continue
  sys.stdout.write("05")
  y += 1
 sys.stdout.write("\n")
 x+=1
