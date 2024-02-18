require_relative 'clipper2rb'

include Clipper2
subject = Paths64.new
clip = Paths64.new
subject << make_path64([100, 50, 10, 79, 65, 2, 65, 98, 10, 21])
clip << make_path64([98, 63, 4, 68, 77, 8, 52, 100, 19, 12])
solution = Clipper2.intersect64(subject, clip, FillRule::NonZero)
puts solution
