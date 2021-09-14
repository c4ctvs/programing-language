#include "../Block.h"
#include "../Expr.h"
#include <catch2/catch.hpp>
#include <string>
#include <utility>

TEST_CASE("Parsing empty block", "[Block]") {
  REQUIRE(*Block::parse("{}")->first == Block({}));
}

TEST_CASE("Parsing empty block with whitespace", "[Block]") {
  REQUIRE(*Block::parse("{  }")->first == Block({}));
}

TEST_CASE("Parsing block with one statment", "[Block]") {
  ExprNumber n(5);
  vector<IStatment *> block = {&n};
//  REQUIRE(Block::parse("{ 5 }")->first == Block(block));
}

// TODO: For now assigment operations evaluate to the value that is being
// assigned
TEST_CASE("Parsing block with multiple statments", "[Block]") {
  Number n(5);
  ExprOperation o(&n, &n, Operator(Operator::Add));
  vector<IStatment *> block = {&n, &o, &n};
  REQUIRE(*Block::parse("{ mat x = 5  5 + 5  5 }")->first == Block(block));
}

TEST_CASE("Parsing block with assigning variable to variable", "[Block]") {
  ExprNumber a(5);
  ExprNumber b(5);
  vector<IStatment *> block = {&a, &b, &b};
  REQUIRE(*Block::parse("{ mat a = 5  mat b = a  b}")->first == Block(block));
}

TEST_CASE("Blocks evaluate to last expression if last expression returns value",
		  "[Block]") {
  Env env;
  REQUIRE(*(Number *)Block::parse("{5}")->first->eval(env).get() == Number(5));
}

TEST_CASE("Blocks evaluate to last expression if last expression returns value "
		  "and is a variable",
		  "[Block]") {
  Env env;
  REQUIRE(*(Number *)Block::parse("{mat a = 1 a}")->first->eval(env).get() == Number(1));
}
