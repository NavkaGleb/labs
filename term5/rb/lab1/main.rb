# variant 7

print "Enter a: "
a = gets.chomp.to_f

print "Enter b: "
b = gets.chomp.to_f

print "Enter: x: "
x = gets.chomp.to_f

print "Enter phi: "
phi = gets.chomp.to_f

result = x * Math.sqrt(phi - a) ** (1 / 4) +
  (Math.log(Math.sin(b).abs) ** 3 + Math.tan(x) ** (1 / 3)) /
    (2.3 * Math.log10(x.abs))

print "result: ", result
