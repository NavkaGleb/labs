# Variant 7

def from_decimal_to_binary(number)
  String binary = ''

  while number != 0
    binary << (number % 2).to_s
    number /= 2
  end

  binary.reverse
end

def main
  puts "binary_form: #{from_decimal_to_binary(332)}"
end

main
