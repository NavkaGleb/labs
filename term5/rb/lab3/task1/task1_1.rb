# Variant 7

def task_a(a, b)
  !(a || b) && (a && !b)
end

def task_b(a, b, c, x, y, z)
  ((z != y).object_id <= (6 >= y).object_id) && a || b && c && x >= 1.5
end

def task_c(x, y, z)
  (8 - x * 2 <= z) && (x ** 2 != y ** 2) || (z >= 15)
end

def task_d(x, y, z)
  x > 0 && y < 0 || z >= (x * y + (-y / x) + -z / 3)
end

def task_e(a, b, c)
  !(a || b && !(c || (!a || b)))
end

def task_f(x, y)
  x ** 2 + y ** 2 >= 1 && x >= 0 && y >= 0
end

def task_g(a, b, c)
  (a && (c && b != b || a) || c) && b
end

def main
  a = false
  b = false
  c = false

  x = -8
  y = -10
  z = -12

  puts "task_a: #{task_a(a, b)}"
  puts "task_b: #{task_b(a, b, c, x, y, z)}"
  puts "task_c: #{task_c(x, y, z)}"
  puts "task_d: #{task_d(x, y, z)}"
  puts "task_e: #{task_e(a, b, c)}"
  puts "task_f: #{task_f(x, y)}"
  puts "task_g: #{task_g(a, b, c)}"

end

main
