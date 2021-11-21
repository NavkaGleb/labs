def interpolation_lagrange_polynomial(x, nodes, function):
    result = 0

    for i in range(len(nodes)):
        numerator = 1

        for j in range(len(nodes)):
            if j == i:
                continue

            numerator *= x - nodes[j]

        denominator = 1

        for j in range(len(nodes)):
            if j == i:
                continue

            denominator *= nodes[i] - nodes[j]

        result += (numerator / denominator) * function(nodes[i])

    return result


def find_lagrange_form(nodes, function):
    return lambda x: interpolation_lagrange_polynomial(x, nodes, function)
