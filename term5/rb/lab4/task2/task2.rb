def gauss(coefficients, constant_terms, n)
  x = (0..n - 1).map{ Array.new(n) }

  (0..n - 1).each do |k|
    
    max = coefficients[k][k].abs
    index = k
    
    (k + 1..n - 1).each do |i|
      if (coefficients[i][k]).abs > max
        max = (coefficients[i][k]).abs
        index = i
      end
    end
    
    (0..n - 1).each do |j|
      temp = coefficients[k][j]
      coefficients[k][j] = coefficients[index][j]
      coefficients[index][j] = temp
    end
    
    temp = constant_terms[k]
    constant_terms[k] = constant_terms[index]
    constant_terms[index] = temp
    
    (k..n - 1).each do |i|
      temp = coefficients[i][k]
      (0..n - 1).each do |j|
        coefficients[i][j] = coefficients[i][j].to_f / temp
      end

      constant_terms[i] = constant_terms[i] / temp
      
      if i == k
        next
      end
      
      (0..n - 1).each do |j|
        coefficients[i][j] = coefficients[i][j] - coefficients[k][j];
      end
      
      constant_terms[i] = constant_terms[i] - constant_terms[k]
    end
    
    puts coefficients.to_a.map(&:inspect)
    puts
  end
  
  (n - 1).downto(0) do |k|
    x[k] = constant_terms[k].round(5)
    
    (0..k).each do |i|
      constant_terms[i] = constant_terms[i] - coefficients[i][k] * x[k];
    end
  end
  
  x
end

def main
  k = 5

  puts "Enter size: "
  size = gets.chomp.to_i

  if size < 3 || size > 9
    throw "Bad size"
  end

  constant_terms = (1..size).map { |i| i.to_f }
  coefficients = (0...size).map { |i|
    Array.new(size) { |j|
      if i == j
        2.to_f
      else
        (k + 2).to_f
      end
    }
  }

  puts "coefficients"
  puts coefficients.to_a.map(&:inspect)
  puts

  puts "constant terms"
  p constant_terms
  puts

  puts "x: #{gauss(coefficients, constant_terms, size)}"
end

main
