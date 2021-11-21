def main
  array = Array.new(12) { rand 10 }

  p array

  groups = 0
  current_number = nil

  array.each_index do |i|
    if i > 0 && array[i] == array[i - 1] && current_number != array[i]
      groups = groups.next
      current_number = array[i]
    else
      current_number = nil
    end
  end

  p array
  print "groups: #{groups}"
end

main
