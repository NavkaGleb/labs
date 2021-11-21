# Variant 7

def factorial(n)
  (1..n).inject(:*) || 1
end

def function(x, i)
  -1 ** i * (x ** (2 * i)) / (factorial(2 * i))
end

def series(x, n)
  x_left_bound = 0.1
  x_right_bound = 1

  n_left_bound = 10
  n_right_bound = 58

  eps = 0.001

  if x < x_left_bound || x > x_right_bound
    raise "Bad x!"
  end

  sum = 0

  if n >= n_left_bound && n <= n_right_bound
    (0..n).each do |i|
      sum += yield(x, i)
    end
  else
    fault = eps + 1
    prev_sum = sum
    i = 0

    while i < 100 || fault > eps
      sum += yield(x, i)

      i = i.next
      fault = (prev_sum - sum).abs
      prev_sum = sum
    end
  end

  sum
end

def main
  puts "sum: #{series(0.5, 100) { |x, i| function(x, i) }}\n"
end

main
