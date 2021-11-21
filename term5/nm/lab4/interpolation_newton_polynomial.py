def separated_differences(nodes, function):
    result = 0

    for i in range(len(nodes)):
        numerator = function(nodes[i])
        denominator = 1

        for j in range(len(nodes)):
            if j == i:
                continue

            denominator *= nodes[i] - nodes[j]

        result += numerator / denominator

    return result


def interpolation_newton_polynomial(x, nodes, function):
    result = 0

    for i in range(len(nodes)):
        term = separated_differences(nodes[:i + 1], function)

        for j in range(i):
            term *= x - nodes[j]

        result += term

    return result


def find_newton_form(nodes, function):
    return lambda x: interpolation_newton_polynomial(x, nodes, function)
