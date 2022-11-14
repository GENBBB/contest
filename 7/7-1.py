a = 9223372036854775807
a = int(a)
a += -1
print(a)
if a % 3 == 0:
    a = a / 3
    print("!", int(a))
    print(int(a*3))
