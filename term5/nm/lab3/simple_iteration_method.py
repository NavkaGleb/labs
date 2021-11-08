# math.tan(x * y + 0.1) = x ** 2
# x ** 2 + 2 * y ** 2 = 1

import math
import numpy as np


def main():
    eps = 0.0001

    x, y = 0, 0

    f1 = 'math.sqrt(math.tan(x * y + 0.1))'
    f2 = 'math.sqrt((1 - x ** 2) / 2)'

    derivative_f1_x = 'y / (math.cos(x * y + 0.1) ** 2)'
    derivative_f1_y = 'x / (math.cos(x * y + 0.2) ** 2)'

    derivative_f2_x = '(math.sqrt(2) * x) / (2 * math.sqrt(1 - x ** 2))'
    derivative_f2_y = '0'

    b = np.array([
        [eval(derivative_f1_x), eval(derivative_f1_y)],
        [eval(derivative_f2_x), eval(derivative_f2_y)]
    ])

    if np.linalg.norm(b, ord=np.inf) >= 1:
        raise Exception('There is no convergence')

    while True:
        previous_x, previous_y = x, y

        x, y = eval(f1), eval(f2)

        print(f'x = {x:0.10f}, y = {y:0.10f}')

        if np.linalg.norm(np.array([x, y]) - np.array([previous_x, previous_y]), ord=np.inf) <= eps:
            break

    print(f'solution: [{x:0.10f} {y:0.10f}]')


if __name__ == '__main__':
    main()
