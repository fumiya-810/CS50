height = 0


while True:
    try:
        height = int(input("Height:"))

        if height > 0 and height < 9:
            break

    except ValueError:
        print("That is not integer")
        pass


for i in range(height):
    for j in range(height - i - 1):
        print(" ", end="")

    for k in range(i + 1):
        print("#", end="")

    print()
