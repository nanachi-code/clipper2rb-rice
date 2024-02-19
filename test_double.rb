require_relative 'clipper2rb'

include Fujilogic::Clipper2
subject = PathsD.new
clip = PathsD.new
subject << make_pathD([100, 50, 10, 79, 65, 2, 65, 98, 10, 21])
clip << make_pathD([98, 63, 4, 68, 77, 8, 52, 100, 19, 12])
solution = intersectD(subject, clip, FillRule::NonZero)
puts solution
