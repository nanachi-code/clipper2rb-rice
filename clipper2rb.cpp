#include "include/clipper2/clipper.h"
#include <rice/rice.hpp>
#include <rice/stl.hpp>
#include <iostream>
#include "include/clipper2/clipper.engine.cpp"
#include "include/clipper2/clipper.offset.cpp"
#include "include/clipper2/clipper.rectclip.cpp"

using namespace Rice;
using namespace Clipper2Lib;

template <typename T>
Array point_to_a(Point<T> self)
{
  Array ary;
  ary.push(self.x);
  ary.push(self.y);
  return ary;
}

template <typename T>
String point_to_s(Point<T> self)
{
  String res(String::format("[%d %d]", self.x, self.y));
  return res;
}

template <typename T>
Point<T> point_from_a(Array ary)
{
  Point<T> p(0, 0);
  // check if both elements are numeric values
  if (ary.size() == 2)
  {
    Object x = ary[0];
    Object y = ary[1];
    Rice::detail::From_Ruby<T> converter;

    if (converter.is_convertible(x) && converter.is_convertible(y))
    {
      p.x = converter.convert(x);
      p.y = converter.convert(y);
    }
  }

  return p;
}

void define_enums(Module &module)
{
  Enum<ClipType> rb_cClipType = define_enum<ClipType>("ClipType", module);
  rb_cClipType.define_value("None", ClipType::None);
  rb_cClipType.define_value("Intersection", ClipType::Intersection);
  rb_cClipType.define_value("Union", ClipType::Union);
  rb_cClipType.define_value("Difference", ClipType::Difference);
  rb_cClipType.define_value("Xor", ClipType::Xor);

  Enum<FillRule> rb_cFillRule = define_enum<FillRule>("FillRule", module);
  rb_cFillRule.define_value("EvenOdd", FillRule::EvenOdd);
  rb_cFillRule.define_value("NonZero", FillRule::NonZero);
  rb_cFillRule.define_value("Positive", FillRule::Positive);
  rb_cFillRule.define_value("Negative", FillRule::Negative);
}

void define_int64_classes_and_methods(Module &module)
{
  Data_Type<Point64> rb_cPoint64 = define_class_under<Point64>(module, "Point64");
  rb_cPoint64.define_constructor(Constructor<Point64, int64_t, int64_t>(), Arg("x") = 0, Arg("y") = 0);
  rb_cPoint64.define_attr("x", &Point64::x);
  rb_cPoint64.define_attr("y", &Point64::y);
  rb_cPoint64.define_method("*", &Point64::operator*);
  rb_cPoint64.define_method("+", &Point64::operator+);
  using Point64SubtractSig = Point64 (Point64::*)(const Point64 &) const;
  rb_cPoint64.define_method<Point64SubtractSig>("-", &Point64::operator-);
  using Point64NegateSig = Point64 (Point64::*)() const;
  rb_cPoint64.define_method<Point64NegateSig>("-@", &Point64::operator-);
  rb_cPoint64.define_method("negate", &Point64::Negate);
  rb_cPoint64.define_method("to_a", &point_to_a<int64_t>);
  rb_cPoint64.define_method("to_s", &point_to_s<int64_t>);
  rb_cPoint64.define_method("inspect", &point_to_s<int64_t>);
  rb_cPoint64.define_singleton_function("from_a", &point_from_a<int64_t>);

  Data_Type<Path64> rb_cPath64 = define_vector_under<Path64>(module, "Path64");

  Data_Type<Paths64> rb_cPaths64 = define_vector_under<Paths64>(module, "Paths64");

  using MakePath64Sig = Path64 (*)(const std::vector<int64_t> &);
  module.define_module_function<MakePath64Sig>("make_path64", &MakePath);

  using Intersect64Sig = Paths64 (*)(const Paths64 &, const Paths64 &, FillRule);
  module.define_module_function<Intersect64Sig>("intersect64", &Intersect);
}

void define_double_classes_and_methods(Module &module)
{
  Data_Type<PointD> rb_cPointD = define_class_under<PointD>(module, "PointD");
  rb_cPointD.define_constructor(Constructor<PointD, int64_t, int64_t>(), Arg("x") = 0, Arg("y") = 0);
  rb_cPointD.define_attr("x", &PointD::x);
  rb_cPointD.define_attr("y", &PointD::y);
  rb_cPointD.define_method("*", &PointD::operator*);
  rb_cPointD.define_method("+", &PointD::operator+);
  using PointDSubtractSig = PointD (PointD::*)(const PointD &) const;
  rb_cPointD.define_method<PointDSubtractSig>("-", &PointD::operator-);
  using PointDNegateSig = PointD (PointD::*)() const;
  rb_cPointD.define_method<PointDNegateSig>("-@", &PointD::operator-);
  rb_cPointD.define_method("negate", &PointD::Negate);
  rb_cPointD.define_method("to_a", &point_to_a<double>);
  rb_cPointD.define_method("to_s", &point_to_s<double>);
  rb_cPointD.define_method("inspect", &point_to_s<double>);
  rb_cPointD.define_singleton_function("from_a", &point_from_a<int64_t>);

  Data_Type<PathD> rb_cPathD = define_vector_under<PathD>(module, "PathD");

  Data_Type<PathsD> rb_cPathsD = define_vector_under<PathsD>(module, "PathsD");

  using MakePathDSig = PathD (*)(const std::vector<double> &);
  module.define_module_function<MakePathDSig>("make_pathD", &MakePathD);

  module.define_module_function(
      "intersectD",
      [](const PathsD &subjects, const PathsD &clips, FillRule fillrule, int decimal_prec = 2) -> PathsD
      {
        return Intersect(subjects, clips, fillrule, decimal_prec);
      },
      Arg("subjects"),
      Arg("clips"),
      Arg("fillrule"),
      Arg("decimal_prec") = 2);
}

extern "C" void Init_clipper2rb()
{
  Module rb_mFujilogic = define_module("Fujilogic");
  Module rb_mClipper2 = define_module_under(rb_mFujilogic, "Clipper2");

  // Define all data structures.
  define_enums(rb_mClipper2);
  define_int64_classes_and_methods(rb_mClipper2);
  define_double_classes_and_methods(rb_mClipper2);
}
