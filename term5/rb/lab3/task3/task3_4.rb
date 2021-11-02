def calculate_expression(n)
  result = 0.0

  1.upto(n) do |i|
    result += 1.0 / (1..i).sum { |j| Math.sin(j) }
  end

  result
end

def main
  puts "Enter n: "
  n = gets.chomp.to_i

  puts "result: #{calculate_expression(n)}"
end

main
