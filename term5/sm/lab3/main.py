import numpy as np


STEP = 0.2
EPS = 0.0001


def calc_sensitivity_matrix(m1, m2, m3, c1, c2, c3, c4):
    return np.array([
        [0,               1, 0,               0, 0,               0],
        [-(c2 + c1) / m1, 0, c2 / m1,         0, 0,               0],
        [0,               0, 0,               1, 0,               0],
        [c2 / m2,         0, -(c2 + c3) / m2, 0, c3 / m2,         0],
        [0,               0, 0,               0, 0,               1],
        [0,               0, c3 / m3,         0, -(c4 + c3) / m3, 0]
    ])


def calc_c1_derivative(m1, m2, m3, c1, c2, c3, c4):
    return np.array([
        [0,       0, 0, 0, 0, 0],
        [-1 / m1, 0, 0, 0, 0, 0],
        [0,       0, 0, 0, 0, 0],
        [0,       0, 0, 0, 0, 0],
        [0,       0, 0, 0, 0, 0],
        [0,       0, 0, 0, 0, 0]
    ])


def calc_c4_derivative(m1, m2, m3, c1, c2, c3, c4):
    return np.array([
        [0, 0, 0, 0, 0,       0],
        [0, 0, 0, 0, 0,       0],
        [0, 0, 0, 0, 0,       0],
        [0, 0, 0, 0, 0,       0],
        [0, 0, 0, 0, 0,       0],
        [0, 0, 0, 0, -1 / m3, 0]
    ])


def calc_m2_derivative(m1, m2, m3, c1, c2, c3, c4):
    return np.array([
        [0,                0, 0,                     0, 0,               0],
        [0,                0, 0,                     0, 0,               0],
        [0,                0, 0,                     0, 0,               0],
        [- c2 / (m2 ** 2), 0, (c2 + c3) / (m2 ** 2), 0, -c3 / (m2 ** 2), 0],
        [0,                0, 0,                     0, 0,               0],
        [0,                0, 0,                     0, 0,               0]
    ])


def calc_model_derivatives(m1, m2, m3, c1, c2, c3, c4, y):
    c1_derivative = calc_c1_derivative(m1, m2, m3, c1, c2, c3, c4) @ y
    c4_derivative = calc_c4_derivative(m1, m2, m3, c1, c2, c3, c4) @ y
    m2_derivative = calc_m2_derivative(m1, m2, m3, c1, c2, c3, c4) @ y

    return np.array([c1_derivative, c4_derivative, m2_derivative]).T


def f(m1, m2, m3, c1, c2, c3, c4, y):
    return calc_sensitivity_matrix(m1, m2, m3, c1, c2, c3, c4) @ y


def model_runge_kutta(m1, m2, m3, c1, c2, c3, c4, measurements):
    y = np.zeros_like(measurements)
    y[0] = measurements[0].copy()

    for i in range(1, len(measurements)):
        k1 = STEP * f(m1, m2, m3, c1, c2, c3, c4, y[i - 1])
        k2 = STEP * f(m1, m2, m3, c1, c2, c3, c4, y[i - 1] + k1 / 2)
        k3 = STEP * f(m1, m2, m3, c1, c2, c3, c4, y[i - 1] + k2 / 2)
        k4 = STEP * f(m1, m2, m3, c1, c2, c3, c4, y[i - 1] + k3)

        y[i] = y[i - 1] + (k1 + 2 * k2 + 2 * k3 + k4) / 6

    return y


def sensitivity_function_runge_kutta(m1, m2, m3, c1, c2, c3, c4, y, measurement_count):
    u = np.zeros([measurement_count, 6, 3])

    a = calc_sensitivity_matrix(m1, m2, m3, c1, c2, c3, c4)
    beta_derivative = calc_model_derivatives(m1, m2, m3, c1, c2, c3, c4, y.T)

    for i in range(1, measurement_count):
        k1 = STEP * (a @ u[i - 1] + beta_derivative[i - 1])
        k2 = STEP * (a @ (u[i - 1] + k1 / 2) + beta_derivative[i - 1])
        k3 = STEP * (a @ (u[i - 1] + k2 / 2) + beta_derivative[i - 1])
        k4 = STEP * (a @ (u[i - 1] + k3) + beta_derivative[i - 1])

        u[i] = u[i - 1] + (k1 + 2 * k2 + 2 * k3 + k4) / 6

    return u


def calc_delta(y, u, measurements):
    lhs = []
    rhs = []

    for i in range(u.shape[0]):
        lhs.append(u[i].T @ u[i] * STEP)
        rhs.append(u[i].T @ (measurements - y)[i] * STEP)

    lhs = np.linalg.inv(np.array(lhs).sum(0))
    rhs = np.array(rhs).sum(0)

    return lhs @ rhs


def calc_beta(b, m1, m3, c2, c3, measurements):
    measurement_count = len(measurements)

    c1 = b[0]
    c4 = b[1]
    m2 = b[2]

    while True:
        # Step 1 (Runge-Kutta for model)
        y = model_runge_kutta(m1, m2, m3, c1, c2, c3, c4, measurements)

        # Step 2 (Runge-Kutta for sensitivity function)
        u = sensitivity_function_runge_kutta(m1, m2, m3, c1, c2, c3, c4, y, measurement_count)

        # Step 3 (Finding delta of beta)
        delta = calc_delta(y, u, measurements)

        # Step 4
        c1 += delta[0]
        c4 += delta[1]
        m2 += delta[2]

        max_delta = np.abs(delta).max()

        print(f'max_delta: {max_delta}')

        # Step 5
        if np.abs(delta).max() < EPS:
            break

    return np.array([c1, c2, m2])


def main():
    c2 = 0.3
    c3 = 0.2
    m1 = 12
    m3 = 18

    # b[0] = c1, b[1] = c4, b[2] = m2
    b0 = np.array([0.1, 0.08, 21])

    measurements = []

    with open('y5.txt') as file:
        for line in file.readlines():
            measurements.append(line.split())

    measurements = np.array(measurements, float).T

    b = calc_beta(b0, m1, m3, c2, c3, measurements)

    print(f'c1: {b[0]}\nc4: {b[1]}\nm2: {b[2]}')


if __name__ == "__main__":
    main()
