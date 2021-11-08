import numpy as np


def find_max_eigenvalue(matrix, eps):
    eigenvalue = 0

    x = np.ones(matrix.shape[0])

    while True:
        previous_eigenvalue = eigenvalue
        e = x / np.linalg.norm(x, ord=np.inf)
        x = matrix @ e
        eigenvalue = (x @ e) / (e @ e)

        if abs(eigenvalue - previous_eigenvalue) <= eps:
            break

    return eigenvalue


def main():
    matrix = np.array([
        [3.0, -1.0, 1.0, 2.0],
        [-1.0, 2.0, 0.5, 1.0],
        [1.0, 0.5, 3.0, 0.5],
        [2.0, 1.0, 0.5, 6.0]
    ])

    eps = 0.01

    max_eigenvalue = find_max_eigenvalue(matrix, eps)

    b = np.dot(np.eye(matrix.shape[0]), max_eigenvalue) - matrix
    min_eigenvalue = max_eigenvalue - find_max_eigenvalue(b, eps)

    print(f'max_eigenvalue = {max_eigenvalue}')
    print(f'min_eigenvalue = {min_eigenvalue}')


if __name__ == '__main__':
    main()
