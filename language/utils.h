#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

using std::nullopt;
using std::optional;
using std::string;
using Str2 = std::pair<string, string>;

#ifndef UTILS_H
#define UTILS_H

#define msg(var) std::cerr << #var << "='" << var << "'" << std::endl;

#define err(message, value)                     \
  do {                                          \
	std::cerr << message << value << std::endl; \
	std::exit(1);                               \
  } while (false)

#define errmsg(message)                \
  do {                                 \
	std::cerr << message << std::endl; \
	std::exit(1);                      \
  } while (false)

static Str2 take_whle(const string &expr, bool (*func)(char z)) {
  int i = 0;
  while (func(expr[i]))
	i++;
  return Str2(expr.substr(0, i), expr.substr(i));
}

static bool is_digit(char c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

static bool is_whitespace(char c) {
  return std::isspace(static_cast<unsigned char>(c)) || c == '\n';
}

static Str2 extract_digits(const std::string expr) {
  return take_whle(expr, is_digit);
}

static Str2 extract_whitespace(const std::string expr) {
  return take_whle(expr, is_whitespace);
}

static optional<Str2> extract_operator(const std::string expr) {
  char op = expr[0];
  if (op == '+' || op == '-' || op == '*' || op == '/') {
	return make_pair(expr.substr(0, 1), expr.substr(1));
  }
  std::string char2 = expr.substr(0, 2);
  if (char2 == "==") {
	return make_pair(char2, expr.substr(2));
  }
  return std::nullopt;
}

static bool is_alphanumeric(char c) { return std::isalpha(c); }

static optional<Str2> tag(const string &text, const string &prefix) {
  if (text.rfind(prefix, 0) == 0) {// pos=0 limits the search to the prefix
	return optional(make_pair(prefix, text.substr(prefix.size())));
  } else {
	return nullopt;
  }
}

static bool is_alphanumeric_or_digit(char c) {
  return is_alphanumeric(c) || is_digit(c);
}

static optional<Str2> extract_identifier(const string &text) {
  // FIXME: First character in id can be an number using that we should not
  // allow it
  auto result = take_whle(text, is_alphanumeric_or_digit);

  if (result.first == "")
	return nullopt;

  if (is_digit(result.first[0]))
	return nullopt;

  return optional(result);
}

#endif
