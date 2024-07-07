#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>

enum class TokenType {
	Unknown,
	EndOfFile,

	// Primary
	Identifier,
	Integer,
	Float,
	String,
	Char,

	// Math Operators
	Plus,
	Minus,
	Times,

	// Conditional Operators
	Equals,
	NotEquals,
	And,
	Or,
	Not,

	// Expression Operators
	Assignment,
	Declaration,

	// Symbols
	Colon,
	Dot,
};

struct Token {
	TokenType type;
	std::string value;
};

Token
GetNextToken(std::string_view &content)
{
	Token token = { TokenType::EndOfFile, "" };

	size_t i;
	for (i = 0; i < content.length(); ++i) {
		auto c = content[i];
		char next_c;
		size_t j;

		if (std::isspace(c))
			continue;

		// Number
		if (std::isdigit(c) || c == '.') {
			auto number_str = std::string(1, c);
			size_t digit_count = 0;
			size_t dot_count = 0;

			for (j = (i + 1); j < content.length(); ++j) {
				next_c = content[j];

				if (std::isdigit(next_c)) {
					++digit_count;
				} else if (next_c == '.') {
					++dot_count;
				} else {
					break;
				}

				number_str.push_back(next_c);
			}
			i = j;

			token.value = number_str;

			// Check if number is valid
			if (digit_count == 0) {
				token.type = TokenType::Unknown;
			}

			switch (dot_count) {
			case 0:
				token.type = TokenType::Integer;
				break;
			case 1:
				token.type = TokenType::Float;
				break;
			default:
				token.type = TokenType::Unknown;
				break;
			}

			break;
		}

		// Identifier
		if (std::isalpha(c)) {
			std::string identifier = std::string(1, c);

			for (j = i + 1; j < content.length(); ++j) {
				next_c = content[j];
				if (!std::isalpha(next_c) && !std::isdigit(next_c) && c != '_') {
					break;
				}

				identifier.push_back(next_c);
			}
			i = j;

			token.type = TokenType::Identifier;
			token.value = identifier;
			break;
		}

		// Unknown
		token.type = TokenType::Unknown;
		token.value = std::string(1, c);

		for (j = i + 1; j < content.length(); ++j) {
			next_c = content[j];
			if (std::isspace(next_c)) {
				break;
			}

			token.value.push_back(next_c);
		}
		i = j;

		break;
	}

	content = content.substr(i);

	return token;
}

void
Parse(std::string_view content)
{
	Token token;

	while (token = GetNextToken(content), token.type != TokenType::EndOfFile) {
		std::cout << "TOKEN: " << static_cast<int>(token.type) << " -> " << token.value << std::endl;
	}
}

int main()
{
	std::cout << "Flow" << std::endl;

	auto file = std::ifstream("basic.flo");
	std::stringstream ss;

	ss << file.rdbuf();
	Parse(ss.str());

	return 0;
}
