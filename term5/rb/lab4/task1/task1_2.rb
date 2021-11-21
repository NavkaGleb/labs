require 'matrix'

def mult_matrix_by_number(array_2d, number)
  rows = array_2d.size
  cols = array_2d[0].size

  (0...rows).each { |i|
    (0...cols).each { |j|
      array_2d[i][j] *= number
    }
  }

  Matrix[*array_2d]
end

def main
  size = 8

  array_2d = (0...size).map { |i|
    Array.new(size) { |j|
      if i == j
        1
      else
        rand 10
      end
    }
  }

  matrix = Matrix[*array_2d]

  puts "matrix before"
  puts matrix.to_a.map(&:inspect)
  puts

  double_matrix = matrix * 2

  puts "matrix after (standard)"
  puts double_matrix.to_a.map(&:inspect)
  puts

  double_matrix = mult_matrix_by_number(array_2d, 2)
  puts "matrix after (custom)"
  puts double_matrix.to_a.map(&:inspect)
  puts
end

main
