#include "../BindDef.h"
#include "../BindingUsage.h"
#include "../Expr.h"
#include "../Null.h"
#include "../env.h"
#include <catch2/catch.hpp>
#include <string>
#include <utility>

TEST_CASE("Parsinng binding operation", "[Parser]") {
  auto expr = ExprOperation(Number(2), Number(4), Operator(Operator::Divide));
  REQUIRE(BindDef::Parse("mat x = 2 / 4").value().first == BindDef("x", &expr));
}

TEST_CASE("Parsinng binding number", "[Parser]") {
  auto expr = ExprNumber(Number(2));
  REQUIRE(BindDef::Parse("mat x = 2 ").value().first == BindDef("x", &expr));
}

TEST_CASE("Parse numbers in var name", "[Parser]") {
  auto expr = ExprOperation(Number(2), Number(4), Operator(Operator::Divide));
  REQUIRE(BindDef::Parse("mat x12 = 2 / 4").value().first ==
          BindDef("x12", &expr));
  REQUIRE(BindDef::Parse("mat x12x = 2 / 4").value().first ==
          BindDef("x12x", &expr));
}

TEST_CASE("Dont parse numbers at the start ov var name", "[Parser]") {
  REQUIRE(BindDef::Parse("mat 12x = 2 / 4") == std::nullopt);
  REQUIRE(BindDef::Parse("mat 12 = 2 / 4") == std::nullopt);
}

TEST_CASE("Dont parse vars without name", "[Parser]") {
  REQUIRE(BindDef::Parse("mat = 2 / 4") == std::nullopt);
}

TEST_CASE("Weird var declaration", "[Parser]") {
  REQUIRE(BindDef::Parse("xmat x = 2 / 4") == std::nullopt);
  REQUIRE(BindDef::Parse("mxat x = 2 / 4") == std::nullopt);
}

TEST_CASE("Dont parse without space between let var name", "[Parser]") {
  REQUIRE(BindDef::Parse("matx = 2 / 4") == std::nullopt);
}

TEST_CASE("Dont parse without var name", "[Parser]") {
  REQUIRE(BindDef::Parse("mat = 2 / 4") == std::nullopt);
}

TEST_CASE("Bind usgae", "[Parser]") {
  auto bu = BindingUsage("x");
  REQUIRE(*BindingUsage::parse("x").value().first == *(&bu));
}

TEST_CASE("Storing binding", "[Parser]") {
  Env env;
  Number number(2);
  env.store_binding("x", &number);
  REQUIRE(BindingUsage("x").eval(env)->type_ == (&number)->type_);
}

TEST_CASE("Cannot read not existent var", "[Parser]") {
  Env env;
  auto n = Null();
  REQUIRE(BindingUsage("x").eval(env)->type_ == (&n)->type_);
}