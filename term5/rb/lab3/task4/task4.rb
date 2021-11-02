# Variant 7

$eps = 0.00001

def factorial(n)
  (1..n).inject(:*) || 1
end

def calculate_first_expression
  n = 2
  result = 0.0

  loop do
    previous_result = result
    result = (factorial(n - 1).to_f / factorial(n + 1)) ** (n * (n + 1))

    if (previous_result - result).abs <= $eps
      break
    end

    n = n.next
  end

  result
end

def calculate_second_expression(x)
  n = 0
  result = 0.0

  loop do
    previous_result = result
    result += (n.even? ? 1 : -1) * (x ** (n * 2)) / factorial(2 * n)

    if (previous_result - result).abs <= $eps
      break
    end

    n = n.next
  end

  result
end

def calculate_third_expression
  n = 1
  result = 0.0

  loop do
    previous_result = result
    result += factorial(4 * n - 1).to_f / (factorial(4 * n - 1) * 3 ** (2 * n) * factorial(2 * n))

    if (previous_result - result).abs <= $eps
      break
    end

    n = n.next
  end

  result
end

def main
  puts "first expression: #{calculate_first_expression}"
  puts "expected second expression: #{Math.cos(1.0)}"
  puts "actual second expression: #{calculate_second_expression(1.0)}"
  puts "third expression: #{calculate_third_expression}"
end

main
