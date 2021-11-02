def calculate_expression
  result = 1.0

  1.upto(8) do |i|
    result += 1.0 / (3.0 ** i)
  end

  result
end

def main
  puts "result: #{calculate_expression}"
end

main
