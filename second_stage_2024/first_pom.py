def equation(x1, y1, x2, y2):
    if x1 == x2:
        return "Прямая вертикальная, уравнение: x = {}".format(x1)
    k = (y2 - y1) / (x2 - x1)
   # Находим свободный член b
    b = y1 - k * x1
    return "y = {}x + {}".format(k, b)

x1 = 7.7
y1 = 4.13
x2 = 2
y2 = 2

print(f"Уравнение прямой: {equation(x1, y1, x2, y2)}")