# Variant 7

$n = 10
$c = 30
$pi = 3.141

def calculate_y(x)
  (
    (
      (
        $c ** 3.0 / 2.0 + 27 + x ** 3.0 / 5.0
      ) /
      (
        Math.sqrt($c) + 3 * x ** 1.0 / 5.0
      ) +
      (
        3 * (32 * x ** 2) ** 1.0 / 10.0
      )
    ) *
    (
      x ** -2
    )
  ) ** $c +
  (
    (
      1.0 + x ** (1 / $c) + x ** 2
    ) /
    (
      ($n - x) + 7.0 / 3.0
    )
  )
end

def calculate_z(x)
  (
    (
      1.0 - Math.cos(4 * x)
    ) /
    (
      Math.cos(2 * x - 1) ** -2
    )
  ) +
  (
    (
      1.0 + Math.cos(4 * x)
    ) /
    (
      Math.sin(2 * x - 1) * -2
    )
  ) +
  Math.tan(
    2 * $pi / 9 - x
  ) ** $c.to_f / $n.to_f
end

def tabulate_y
  result = {}
  step = ($n - 1).to_f / ($n + $c).to_f

  (1.0..$n.to_f).step(step) do |x|
    result[x] = calculate_y(x)
  end

  result
end

def tabulate_z
  result = {}

  x = $pi / $n
  finish = $pi
  step = (finish - x) / ((3.0 / 2.0) * $n + $c)

  while x <= $pi
    result[x] = calculate_z(x)
    x += step
  end

  result
end

def tabulate_f
  result = {}
  step = ($c - 2).to_f / (2 * $n)

  (2.0..$c.to_f).step(step) do |x|
    if x > 2 && x < $n
      result[x] = calculate_y(x)
    elsif x > $n && x < 2 * $n
      result[x] = calculate_z(x)
    else
      result[x] = calculate_y(x) + calculate_z(x)
    end
  end

  result
end

def main
  def puts_result(result)
    result.each do |key, value|
      puts "{ #{key}: #{value} }"
    end
  end

  puts "tabulate y"
  puts_result(tabulate_y)

  puts "tabulate z"
  puts_result(tabulate_z)

  puts "tabulate f"
  puts_result(tabulate_f)
end

main
