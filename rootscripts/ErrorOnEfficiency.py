import math
eff = input("Efficiency = ")
nEv = input("# triggered Events = ")

eff = float(eff)
nEv = float(nEv)

error = math.sqrt(eff*(1-eff)/nEv)
print("Error on efficiency = {}".format(error))
