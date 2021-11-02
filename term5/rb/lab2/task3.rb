# Variant 7

def main
  binary_form = 0b0001_1111_0001_1001.to_s(2).split('')
  decimal_form = 0

  binary_form.reverse.each_with_index do |char, index|
    puts "#{index}: #{char}"

    decimal_form += (char.to_i * 2) ** index
  end

  puts "decimal_form: #{decimal_form}"
end

main
