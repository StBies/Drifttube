import math
p0 = input("Enter p0 = ")
sp0 = input("Enter the error on p0 = ")
p1 = input("Enter p1 = ")
sp1 = input("Enter the error on p1 = ")
p0 = float(p0)
sp0 = float(p0)
p1 = float(p1)
sp1 = float(sp1)

maxDrift = p0/p1
maxDriftError = math.sqrt((math.pow(sp0,2) / math.pow(p1,2)) + (math.pow(p0,2)*math.pow(sp1,2) / math.pow(p1,4)))

print("Maximum drift time = {}".format(p0/p1))
print("Error = {}".format(maxDriftError))
