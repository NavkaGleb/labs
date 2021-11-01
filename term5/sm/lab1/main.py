import matplotlib.pyplot as plt
import numpy as np

from scipy.signal import argrelextrema


measurements = np.array(open("f5.txt").read().split(), float)

t = 5
dt = 0.01
time = np.arange(0, t + dt, dt)


def main():
    # Discrete Fourier transform
    n = time.shape[0]
    n_range = np.arange(n)
    k = n_range.reshape((n, 1))
    m = np.exp(-2j * np.pi * k * n_range / n)

    transformed_data = (m @ measurements) / n
    transformed_half_data = transformed_data[:transformed_data.shape[0] // 2 - 1]

    # Main frequency
    main_frequency = np.array(argrelextrema(transformed_half_data, np.greater))[0][0] / t

    print(f'main_frequency: {main_frequency}')

    # Least square method
    c = np.array([
        np.sum(measurements * time ** 3),
        np.sum(measurements * time ** 2),
        np.sum(measurements * time),
        np.sum(measurements * np.sin(2. * np.pi * main_frequency * time)),
        np.sum(measurements)
    ])

    a = np.zeros((c.shape[0], c.shape[0]))

    functions = [
        time ** 3,
        time ** 2,
        time, np.sin(2.0 * np.pi * main_frequency * time),
        np.ones(n)
    ]

    for i in range(a.shape[0]):
        for j in range(a.shape[1]):
            a[i, j] = np.sum(functions[i] * functions[j])

    # Find the solution
    solution = np.linalg.inv(a) @ c

    print(f'solution: {solution}')
    approximated_functions = solution @ functions

    # Draw a chart
    figure, axis = plt.subplots(3)
    figure.suptitle('Data, Approximation, Fourier transforms')

    axis[0].plot(time, measurements)
    axis[1].plot(time, approximated_functions)
    axis[2].plot(time, np.real(transformed_data))

    plt.show()


if __name__ == '__main__':
    main()
