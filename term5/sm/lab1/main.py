import matplotlib.pyplot as plt
import numpy as np

from scipy.signal import argrelextrema


def main1(time, dt):
    t = np.arange(0, time + dt, dt)

    print(t.reshape)


def main():
    # Reading data from file
    data_str = open("f5.txt").read().split()
    data = np.array(data_str, float)

    # Preparing array with timestamps
    t = 5
    dt = 0.01
    time = np.arange(0, t + dt, dt)

    # Finding Fourier transforms
    n = time.shape[0]
    n_range = np.arange(n)
    k = n_range.reshape((n, 1))
    m = np.exp(-2j * np.pi * k * n_range / n)

    transformed_data = np.dot(m, data) / n
    transformed_half_data = transformed_data[:transformed_data.shape[0] // 2 - 1]

    # Find maximums
    maximums = np.array(argrelextrema(transformed_half_data, np.greater))
    main_frequency = maximums[0][0] / t

    print(f'main_frequency: {main_frequency}')

    # Apply least squares method to find resulted approximation
    b = np.array([
        np.sum(data * time ** 3),
        np.sum(data * time ** 2),
        np.sum(data * time),
        np.sum(data * np.sin(2. * np.pi * main_frequency * time)),
        np.sum(data)
    ])

    # Init
    a = np.zeros((b.shape[0], b.shape[0]))

    functions = [
        time ** 3,
        time ** 2,
        time, np.sin(2.0 * np.pi * main_frequency * time),
        np.ones(n)
    ]

    for i in range(b.shape[0]):
        for j in range(b.shape[0]):
            a[i, j] = np.sum(functions[i] * functions[j])

    solution = np.matmul(np.linalg.inv(a), b.T)

    print(f'solution: {solution}')
    approximated_functions = np.dot(solution, functions)

    # Displaying data from all steps
    fig, axs = plt.subplots(3)
    plt.grid(True)
    fig.suptitle('Data, Approximation, Fourier transforms')

    axs[0].plot(time, data)
    axs[1].plot(time, approximated_functions)
    axs[2].plot(time, np.real(transformed_data))

    plt.show()


if __name__ == '__main__':
    main()
