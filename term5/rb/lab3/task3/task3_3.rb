def calculate_expression(n, x)
  x = 2
  result = 1.0

  1.upto(n) do |i|
    result += (x ** i) / (1..i).inject(:*) || 1
  end

  result
end

def main
  puts "Enter n: "
  n = gets.chomp.to_i

  puts "result: #{calculate_expression(n, 2.0)}"
end

main
