# Variant 7

def main
  p = 4
  t = 16
  r = 8

  range = p ** r * (1 - p ** -t).round

  puts "range: #{range}"
end

main
