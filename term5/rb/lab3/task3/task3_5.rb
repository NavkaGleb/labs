def calculate_expression(n)
  result = 0.0

  1.upto(n) do
    result = Math.sqrt(2 + result)
  end

  result
end

def main
  puts "Enter n: "
  n = gets.chomp.to_i

  puts "result: #{calculate_expression(n)}"
end

main
