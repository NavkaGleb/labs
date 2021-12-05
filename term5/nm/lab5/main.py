import matplotlib.pyplot as plt
import numpy as np
import math


def get_a_matrix(size, h):
    a_matrix = np.zeros([size - 2, size - 2])

    for i in range(0, size - 2):
        a_matrix[i][i] = (h[i] + h[i + 1]) / 3

        if i != 0:
            a_matrix[i][i - 1] = h[i] / 6

        if i != size - 3:
            a_matrix[i][i + 1] = h[i + 1] / 6

    return a_matrix


def get_h_matrix(size, h):
    h_matrix = np.zeros([size - 2, size])

    for i in range(0, size - 2):
        h_matrix[i][i] = 1 / h[i]
        h_matrix[i][i + 1] = -1 / h[0] - 1 / h[1]
        h_matrix[i][i + 2] = 1 / h[1]

    return h_matrix


def get_m_vector(y, h):
    size = len(y)

    a = get_a_matrix(size, h)
    h = get_h_matrix(size, h)

    return np.concatenate(([0], np.linalg.inv(a) @ h @ y, [0]))


def get_spline_function(param, i, m, x, y, h):
    i = i + 1

    result = m[i - 1] * (x[i] - param) ** 3 / (6 * h[i - 1])
    result += m[i] * (param - x[i - 1]) ** 3 / (6 * h[i - 1])
    result += (y[i - 1] - (m[i - 1] * h[i - 1] ** 2) / 6) * ((x[i] - param) / h[i - 1])
    result += (y[i] - (m[i] * h[i - 1] ** 2) / 6) * ((param - x[i - 1]) / h[i - 1])

    return result


def get_cubic_spline(x, y, h):
    m = get_m_vector(y, h)

    return lambda param, i: get_spline_function(param, i, m, x, y, h)


def get_piecewise_interpolation_polynomial_2_degree_function(param, i, x, y, h):
    result = y[i - 1] * (param - x[i]) * (param - x[i + 1]) / (2 * h ** 2)
    result -= y[i] * (param - x[i - 1]) * (param - x[i + 1]) / h ** 2
    result += y[i + 1] * (param - x[i - 1]) * (param - x[i]) / (2 * h ** 2)

    return result


def get_piecewise_interpolation_polynomial_2_degree(x, y, h):
    return lambda param, i: get_piecewise_interpolation_polynomial_2_degree_function(param, i, x, y, h)


def function(x):
    return math.sin(0.1 * x + math.e ** (0.3 * x))


def main():
    begin = 0
    end = 10
    node_count = 10

    nodes, step = np.linspace(begin, end, node_count, retstep=True)

    y = np.array([function(node) for node in nodes])
    h = np.full(len(nodes) - 1, step)

    fig = plt.figure(dpi=150)
    fig.subplots_adjust(
        left=0.07,
        bottom=0.07,
        right=0.93,
        top=0.93,
        wspace=0.3,
        hspace=0.4
    )

    # 1
    ax = fig.add_subplot(2, 2, 1)
    ax.set_title('nodes function')
    plt.plot(nodes, y)

    # 2
    ax = fig.add_subplot(2, 2, 2)
    ax.set_title('function')

    for i in range(len(nodes) - 1):
        x_axis = np.linspace(nodes[i], nodes[i + 1])
        y_axis = [function(x) for x in x_axis]
        plt.plot(x_axis, y_axis)

    # 3
    ax = fig.add_subplot(2, 2, 3)
    ax.set_title('spline')

    cubic_spline = get_cubic_spline(nodes, y, h)

    for i in range(len(nodes) - 1):
        x_axis = np.linspace(nodes[i], nodes[i + 1])
        y_axis = [cubic_spline(x, i) for x in x_axis]
        plt.plot(x_axis, y_axis)

    # 3
    ax = fig.add_subplot(2, 2, 4)
    ax.set_title('interpolation polynomial 2 degree')

    interpolation_polynomial = get_piecewise_interpolation_polynomial_2_degree(nodes, y, h[0])

    for i in range(0, len(nodes) - 2, 2):
        x_axis = np.linspace(nodes[i], nodes[i + 2])
        y_axis = [interpolation_polynomial(x, i + 1) for x in x_axis]
        plt.plot(x_axis, y_axis)

    plt.show()


if __name__ == '__main__':
    main()
