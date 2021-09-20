#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "parser.h"

Interpreter interpret_file(const std::string& filepath,
						   Interpreter parser = Interpreter()) {

  std::fstream file;

  file.open(filepath, std::ios::in);

  if (!file.good()) {
	std::cout << "Error(Cannot load file: " << filepath << ")" << std::endl;
	return parser;
  }

  std::string whole_file;
  while (!file.eof()) {
	std::string line;
	std::getline(file, line);
	whole_file.append(line);
  }
  parser.parse(whole_file);
  return parser;
}

Interpreter repl(Interpreter parser = Interpreter()) {
  std::cout << "=== Unnamed programing languge ===" << std::endl;
  std::cout << "Compiled with " << COMPILER << " on " << __DATE__ << " "
			<< __TIME__ << std::endl;

  while (true) {
	std::cout << "=> ";
	std::cout.flush();
	std::string line;
	std::getline(std::cin, line);

	auto tokens = tokenize(line, " ");
	for (int i = 0; i < tokens.size(); i++) {
	  if (tokens[i] == "@q")
		return parser;

	  if (tokens[i] == "@load") {
		if (i + 1 < tokens.size()) {
		  parser = interpret_file(tokens[i + 1], parser);
		  i++;
		  line = "";
		} else {
		  std::cout << "Error(Load requires filapath as a second argument)"
					<< std::endl;
		  line = "";
		}
	  }
	}

	parser.parse(line, true);
  }
}

int main(int argc, char** argv) {

  if (argc == 2) {
	interpret_file(argv[1]);
	return 0;
  }

  if (argc == 3)
	if (std::string(argv[1]) == "-l") {
	  repl(interpret_file(argv[2]));
	  return 0;
	}

  repl();
  return 0;
}
