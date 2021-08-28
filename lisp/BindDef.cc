#include "BindDef.h"

optional<BindDef> BindDef::Parse(const string &text) {

  // Variiable declaration needs to start with "mat"
  auto tag1 = tag(text, "mat");
  if (!tag1)
    return nullopt;

  auto str = tag1.value().second;

  // Remove following whitespace
  str = extract_whitespace(str).second;

  // Extract the name of the variable
  auto name_parse = extract_identifier(str);
  if (!name_parse)
    return nullopt;

  str = name_parse.value().second;

  // Save name of the var for later
  auto bind_name = name_parse.value().first;

  str = extract_whitespace(str).second;

  // Now there needs to be a "="
  auto tag2 = tag(str, "=");
  if (!tag2)
    return nullopt;

  str = tag2.value().second;

  str = extract_whitespace(str).second;

  // Parse an expresion that should be assigned to var
  auto expr_parse = Expr::Parse(str);
  if (!expr_parse)
    return nullopt;

  Expr* bind_expr = expr_parse.value();

  // Create var
  return optional(BindDef(bind_name, bind_expr));
}

BindDef::BindDef(const std::string &name,Expr* expr)
    : name_(name), expr_(expr) {}

bool BindDef::operator==(const BindDef &other) const {
  return name_ == other.name_ && ((Number*)expr_->eval())->get_value() == ((Number*)other.expr_->eval())->get_value();
}

void BindDef::eval(Env &env) { env.store_binding(name_, this->expr_->eval()); }
