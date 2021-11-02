def calculate_expression(x)
  result = 1.0

  1.upto(10) do |i|
    result += (i + 1.0) / (i + 2.0) * x ** i * (i.even? ? 1 : -1)
  end

  result
end

def main
  puts "result: #{calculate_expression(2)}"
end

main
