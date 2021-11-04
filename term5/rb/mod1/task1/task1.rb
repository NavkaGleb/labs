# Variant 7

def function(a, b, c, x)
  if x < 5 && c != 0
    -a * x ** 2 - b
  elsif x > 5 && c != 0
    (x - a) / x
  else
    -x / c
  end
end

def main
  print "Enter a: "
  a = gets.chomp.to_f

  print "Enter b: "
  b = gets.chomp.to_f

  print "Enter c: "
  c = gets.chomp.to_f

  print "Enter x_begin: "
  x_begin = gets.chomp.to_f

  print "Enter x_end: "
  x_end = gets.chomp.to_f

  print "Enter x_step: "
  x_step = gets.chomp.to_f

  if (a.truncate | b.truncate) ^ (a.truncate | c.truncate) % 2 == 0
    a = a.truncate
    b = b.truncate
    c = c.truncate

    x_begin = x_begin.truncate
    x_end = x_end.truncate
    x_step = x_step.truncate
  end

  result = {}

  (x_begin..x_end).step(x_step).each do |x|
    result[x] = function(a, b, c, x)
  end

  puts "result"

  result.each do |key, value|
    puts "#{key} -> #{value}"
  end
end

main
