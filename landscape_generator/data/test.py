import matplotlib.pyplot as plt
import numpy as np

# Задаем коэффициенты a, b, c (замените их своими значениями)
a = 1
b = -2
c = 3


# Функция квадратичной аппроксимации
def f(x):
    return a * x ** 2 + b * x + c


# Загрузка табличных данных
data = np.loadtxt('study1.txt')  # Замените 'data.txt' на имя вашего файла данных

# Извлекаем значения x и y
x = data[:, 0]
y = data[:, 1]

print(x)
# Построение графика данных и аппроксимирующей функции
plt.scatter(x, y, label='Исходные данные')
plt.plot(x, f(x), label='Аппроксимирующая функция', color='red')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()

# Включение отображения сетки
plt.grid(True)


# Сохранение графика в формате SVG
# plt.savefig('график.svg', format='svg')

# Отображение графика
plt.show()
