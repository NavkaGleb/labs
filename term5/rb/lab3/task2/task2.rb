def expression_using_if(x)
  if x > -4.0 && x <= 0.0
    ((x - 2).abs / (x ** 2 * Math.cos(x))) ** (1 / 7.0)
  end

  if x > 0 && x <= 12
    1 / (Math.tan(x + Math.exp(-x)) / Math.sin(x) ** 2) ** (7 / 2).to_f
  end

  if x < -4 or x > 12
    1 / (1 + x / (1 + x / (1 + x)))
  end
end

def expression_using_case(x)
  case x
  when -4..0
    ((x - 2).abs / (x ** 2 * Math.cos(x))) ** (1 / 7.0)
  when 0..13
    1 / (Math.tan(x + Math.exp(-x)) / Math.sin(x) ** 2) ** (7 / 2).to_f
  else
    1 / (1 + x / (1 + x / (1 + x)))
  end
end

def main
  puts "using_if: #{expression_using_if(1)}"
  puts "using_case: #{expression_using_case(1)}"
end

main
