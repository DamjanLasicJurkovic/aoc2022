with open('input.txt', 'r') as file:
    d = file.read()
    
pairs = [[eval(x) for x in a.split("\n")] for a in d.split("\n\n")]

def cmp(x, y):
    if (isinstance(x, list) and isinstance(y, list)):
        i = 0;
        while(True):
            if (len(x) > i and len(y) > i):
                res = cmp(x[i], y[i])
                if (res != 0):
                    return res
            elif (len(x) > i):
                return  -1
            elif (len(y) > i):
                return  1
            else:
                return 0
            i += 1
    elif (isinstance(x, list)):
        return cmp(x, [y])
    elif (isinstance(y, list)):
        return cmp([x], y)
    else:
        if (x==y):
            return 0
        if (x<y):
            return 1
        if (x>y):
            return -1

sum = 0
for i in range(len(pairs)):
    p = pairs[i]
    if (cmp(p[0], p[1]) == 1):
        sum += (i+1)

print("Sum indexes:", sum)

pairs.append([[[2]],[[6]]])
all_packets = []
for p in pairs:
    all_packets.extend(p)

def comp(x,y):
    return cmp(y,x)

from functools import cmp_to_key
all_packets.sort(key=cmp_to_key(comp))
print("Multiplied index of div packets:",(all_packets.index([[2]]) + 1)*(all_packets.index([[6]]) + 1))
