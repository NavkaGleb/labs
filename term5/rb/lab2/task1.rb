# Variant 7

def main
  Array points = [
    [514,  19], [515,  56], [506, 107], [492, 154], [386, 186],
    [314, 185], [283, 234], [205, 233], [134, 220], [ 78, 202],
    [ 68, 179], [ 67, 148], [ 82, 118], [ 90,  94], [ 92,  59],
    [ 94,  42], [107,  29], [155,  32], [199,  60], [224,  51],
    [245,  37], [279,  31], [306,  32], [342,  28], [365,  22],
    [397,  16], [443,  26], [468,  39], [488,  32],
  ]

  square = 0

  points.each_with_index do |_, index|
    next_index = index.next % points.length
    square += (points[index][0] + points[next_index][0]) * (points[index][1] - points[next_index][1])
  end

  square = square.abs / 2

  puts "square: #{square}"
end

main
