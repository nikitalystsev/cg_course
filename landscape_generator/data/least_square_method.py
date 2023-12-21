import math as m

import numpy as np


class LeastSquareMethod:
    """
    Класс аппроксимации табличных значений некоторой квадратичной функцией
    методом наименьших квадратов
    """

    def __init__(
            self,
            filename: str  # путь к файлу с данными
    ) -> None:
        self.__filename = filename

        self.__arr_x: list[float] = list()
        self.__arr_y: list[float] = list()

        self._n: int = 0

        self.__read_file()

        self.__square_interp()
        self.__power_func_interp()
        self.__exponen_func_interp()

    def __read_file(self) -> None:
        """
        Метод парсит файл с датасетом
        :return:
        """

        with open(file=self.__filename) as file:
            self._n = 0
            for row in file:
                data = list(map(float, row.split()))
                self.__arr_x.append(data[0])
                self.__arr_y.append(data[1])
                self._n += 1

    def __square_interp(self):
        """
        Интерполяция квадратичной функцией
        :return:
        """

        one = 0
        for x in self.__arr_x:
            one += (x ** 4)

        two = 0
        for x in self.__arr_x:
            two += (x ** 3)

        three = 0
        for x in self.__arr_x:
            three += (x ** 2)

        four = 0
        for x in self.__arr_x:
            four += x

        five = 0
        for i, x in enumerate(self.__arr_x):
            five += ((x ** 2) * self.__arr_y[i])

        six = 0
        for i, x in enumerate(self.__arr_x):
            six += (x * self.__arr_y[i])

        seven = 0
        for y in self.__arr_y:
            seven += y

        mtr_coeff: np.ndarray = np.array([[one, two, three], [two, three, four], [three, four, self._n]])
        vec_target: np.ndarray = np.array([five, six, seven])

        solve = np.linalg.solve(mtr_coeff, vec_target)

        residual = 0
        for i in range(self._n):
            residual += (solve[0] * (self.__arr_x[i] ** 2) + solve[1] * self.__arr_x[i] + solve[2] - self.__arr_y[
                i]) ** 2

        print("Невязка при аппроксимации квадратичной функцией: ", residual)

    def __power_func_interp(self):
        """
        Интерполяция степенной функцией
        :return:
        """

        one = 0
        for x in self.__arr_x:
            one += m.log(x)

        two = 0
        for y in self.__arr_y:
            two += m.log(y)

        three = 0
        for x in self.__arr_x:
            three += (m.log(x) ** 2)

        four = 0
        for i in range(self._n):
            four += (m.log(self.__arr_x[i]) * m.log(self.__arr_y[i]))

        mtr_coeff: np.ndarray = np.array([[three, one], [one, self._n]])
        vec_target: np.ndarray = np.array([four, two])

        solve = np.linalg.solve(mtr_coeff, vec_target)

        b = m.exp(solve[1])

        residual = 0
        for i in range(self._n):
            residual += (b * (self.__arr_x[i] ** solve[0]) - self.__arr_y[i]) ** 2

        print("Невязка при аппроксимации степенной функцией: ", residual)

    def __exponen_func_interp(self):
        """
        Интерполяция показательной функцией
        :return:
        """
        one = 0
        for x in self.__arr_x:
            one += (x ** 2)

        two = 0
        for y in self.__arr_y:
            two += m.log(y)

        three = 0
        for x in self.__arr_x:
            three += x

        four = 0
        for i in range(self._n):
            four += (self.__arr_x[i] * m.log(self.__arr_y[i]))

        mtr_coeff: np.ndarray = np.array([[one, three], [three, self._n]])
        vec_target: np.ndarray = np.array([four, two])

        solve = np.linalg.solve(mtr_coeff, vec_target)

        b = m.exp(solve[1])

        residual = 0
        for i in range(self._n):
            residual += (b * (m.exp(solve[0] * self.__arr_x[i]) - self.__arr_y[i])) ** 2

        print("Невязка при аппроксимации показательной функцией: ", residual)


def main() -> None:
    """
    Главная функция
    :return:
    """
    LeastSquareMethod("study1.txt")


if __name__ == "__main__":
    main()
