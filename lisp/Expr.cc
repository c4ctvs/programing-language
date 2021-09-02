#include "Expr.h"
#include "BindingUsage.h"
#include <utility>

ExprNumber::ExprNumber(const Number &num) : value_(num) {}

Value *eval();

bool ExprNumber::operator==(const ExprNumber &other) const {
  return value_ == other.value_;
}

std::ostream &operator<<(std::ostream &os, const ExprNumber &e) {
  os << "Expr(" << e.value_ << ")";
  return os;
}

ExprOperation::ExprOperation(const Number &lhs, const Number &rhs,
                             const Operator &op)
    : rhs_(rhs), lhs_(lhs), op_(op) {}

Value *ExprNumber::eval() { return new Number(value_); }

Value *ExprNumber::eval(Env &env) { return this->eval(); }

bool ExprOperation::operator==(const ExprOperation &other) const {
  bool lhs_eq = this->lhs_ == other.lhs_;
  bool rhs_eq = this->rhs_ == other.rhs_;
  bool op_eq = this->op_ == other.op_;
  return lhs_eq && rhs_eq && op_eq;
}

Value *ExprOperation::eval() {
  double result;

  switch (op_.type()) {
  case Operator::Add: {
    result = lhs_.get_value() + rhs_.get_value();
    break;
  }
  case Operator::Subtract: {
    result = lhs_.get_value() - rhs_.get_value();
    break;
  }
  case Operator::Multiply: {
    result = lhs_.get_value() * rhs_.get_value();
    break;
  }
  case Operator::Divide: {
    result = lhs_.get_value() / rhs_.get_value();
    break;
  }
  }
  return new Number(result);
}

Value *ExprOperation::eval(Env &env) { return this->eval(); }

std::ostream &operator<<(std::ostream &os, const ExprOperation &e) {
  os << "Expr(" << e.lhs_ << "," << e.rhs_ << "," << e.op_ << ")";
  return os;
}

ExprVariable::ExprVariable(BindingUsage *b) : variable_(b) {}

Value *ExprVariable::eval() {
  std::cout << "using var without contex" << std::endl;
  return nullptr;
}

Value *ExprVariable::eval(Env &env) { return variable_->eval(env); }

bool ExprVariable::operator==(const ExprVariable &other) const {
  return *variable_ == *other.variable_;
}

optional<pair<Expr *, string>> Expr::Parse(const string &expr) {

  auto str = expr;

  auto num1_parse = Number::Parse(str);
  if (num1_parse) {

    str = num1_parse.value().second;

    str = extract_whitespace(str).second;

    auto op_parse = Operator::Parse(str);

    if (op_parse) {

      str = op_parse.value().second;

      str = extract_whitespace(str).second;

      auto num2_parse = Number::Parse(str);
      if (!num2_parse)
        return nullopt;

      auto lhs = num1_parse.value().first;
      auto op = op_parse.value().first;
      auto rhs = num2_parse.value().first;

      return optional(std::make_pair(new ExprOperation(lhs, rhs, op),
                                     num2_parse.value().second));
    }

    // Could not parse equation we need to backtrack and parse number
    return optional(std::make_pair(new ExprNumber(num1_parse.value().first),
                                   num1_parse.value().second));
  } else {
    if (auto variable = BindingUsage::parse(str)) {
      return optional(std::make_pair(new ExprVariable(variable.value().first),
                                     variable.value().second));
    }
  }
  return nullopt;
}