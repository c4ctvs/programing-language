//
// Created by mateu on 6/4/2021.
//

#ifndef UI4_PROGRAMOWANIE_OBIEKTOWE_SCOPE_H
#define UI4_PROGRAMOWANIE_OBIEKTOWE_SCOPE_H

#include <string>
#include "Token.h"

class Scope : public Token {
    std::string name_;

public:
    Scope(std::string name) : name_(name) {}

    const std::string &GetName() override;

    TokenType GetType() override;

};

#endif //UI4_PROGRAMOWANIE_OBIEKTOWE_SCOPE_H
