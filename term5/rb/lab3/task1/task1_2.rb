# Variant 7

def expression(n, p)
  (Math.exp(n * Math.log(2)) < n ** 2) || !(p || !p)
end

def main
  n = 1
  p = false

  puts "expression: #{expression(n, p)}"
end

main
