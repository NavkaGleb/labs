# Variant 7

def integrate_rectangle(a, b, n, dx = (b - a) / n.to_f)
  x = a + dx / 2
  sum = 0

  (1..n).each do
    y = yield(x)
    sum += dx * y
    x += dx
  end

  sum
end

def integrate_trapezoid(a, b, n, dx = (b - a) / n.to_f)
  x = a + dx
  sum = dx * (yield(a) / 2 - yield(b) / 2)

  loop do
    y = yield(x)
    sum += dx * y
    x += dx

    break if x > b
  end

  sum
end

def main
  f1 = Proc.new do |x|
    x ** 3 * Math::E ** (2 * x)
  end

  f2 = Proc.new do |x|
    Math.tan(5.0 / x + Math::PI / 4) ** (5.0 / 2.0)
  end

  a1 = 0.0
  b1 = 0.8

  a2 = 0.0
  b2 = Math::PI / 8

  print("f1 - rectangle method: #{integrate_rectangle(a1, b1, 100) { |x| f1.call(x) }}\n")
  print("f1 - trapezoid method: #{integrate_trapezoid(a1, b1, 100) { |x| f1.call(x) }}\n")

  print("f2 - rectangle method: #{integrate_rectangle(a2, b2, 100) { |x| f2.call(x) }}\n")
  print("f2 - trapezoid method: #{integrate_trapezoid(a2, b2, 100) { |x| f2.call(x) }}\n")
end

main
