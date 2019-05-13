#include "idecl.h"
#include <sstream>
#include <deque>
#include <unordered_map>

namespace ilib
{
	using std::stringstream;
	using std::deque;
	using std::unordered_map;

	static stringstream ss;

	enum class token_t { 
		ID, NUM, PTR, PARENS, BRACKETS, R_BRAC, L_PAREN, R_PAREN, END,
		VOID, INT, CHAR, LONG, FLOAT, DOUBLE, 
	};
	unordered_map<string, token_t> str2token =
	{
		{ "void", token_t::VOID },
		{"int", token_t::INT},
		{"char", token_t::CHAR},
		{"long", token_t::LONG},
		{"float", token_t::FLOAT},
		{"double", token_t::DOUBLE},
	};
	unordered_map<token_t, string> token2str =
	{
		{ token_t::VOID, "void" },
		{ token_t::INT, "int" },
		{ token_t::CHAR, "char" },
		{ token_t::LONG, "long" },
		{ token_t::FLOAT, "float" },
		{ token_t::DOUBLE, "double" },
	};

	token_t getToken(const char* str, int &index);
	deque<token_t> tokenize(const char* str);
	void dcl(deque<token_t> &tokens);
	void dirdcl(deque<token_t> &tokens);

	const string _idecltype(string decl)
	{
		ss.str("");
		ss.clear();
		auto tokens = tokenize(decl.c_str());
		token_t t = tokens.front();
		tokens.pop_front();
		dcl(tokens);
		ss << " " << token2str[t];
		return ss.str();
	}


	token_t getToken(const char* str, int &index)
	{
		token_t t;
		while (isblank(str[index]))
			++index;

		switch (str[index])
		{
		case '*':
			t = token_t::PTR;
			break;
		case '(':
			if (str[index + 1] == ')')
			{
				++index;
				t = token_t::PARENS;
			}
			else
				t = token_t::L_PAREN;
			break;
		case ')':
			t = token_t::R_PAREN;
			break;
		case '[':
			/*just ignore the cotent in the brackets*/
			token_t tmp;
			++index;
			while ((tmp = getToken(str,index)) != token_t::R_BRAC)
				if (tmp == token_t::END)
					throw string("mismatch on '[', expect a ']'");
			--index;
			t = token_t::BRACKETS;
			break;
		case ']':
			t = token_t::R_BRAC;
			break;
		case '\0':
			t = token_t::END;
			break;
		default:
			int start = index;
			if (isalpha(str[index]))
			{
				while (isalnum(str[++index]))
					;
				string id(str, start, index - start);
				if (str2token.find(id) == str2token.end())
					t = token_t::ID;
				else
					t = str2token[id];

			}
			else if (isdigit(str[index]))
			{
				while (isalnum(str[++index]))
					;
				t = token_t::NUM;
			}
			else
				throw string("syntax error");
			--index;
			break;
		}

		++index;
		return t;
	}

	deque<token_t> tokenize(const char* str)
	{
		deque<token_t> tokens;
		token_t t;
		int index = 0;
		while ((t = getToken(str, index)) != token_t::END)
			tokens.push_back(t);
		tokens.push_back(token_t::END);
		return tokens;
	}

	void dcl(deque<token_t> &tokens)
	{
		int np = 0;
		while (tokens.front() == token_t::PTR)
		{
			++np;
			tokens.pop_front();
		}
		dirdcl(tokens);
		while (np-- > 0)
			ss << " pointer to";
	}

	void dirdcl(deque<token_t> &tokens)
	{
		switch (tokens.front())
		{
		case token_t::L_PAREN:
			tokens.pop_front();
			dcl(tokens);
			if (tokens.front() != token_t::R_PAREN)
				throw string("mismatch on '(', expect a ')'");
			tokens.pop_front();
			break;
		case token_t::ID:
			/*no action*/
			tokens.pop_front();
			break;
		case token_t::END:
			return;
		}
		token_t t;
		while ((t = tokens.front()) == token_t::PARENS || t == token_t::BRACKETS)
		{
			if (t == token_t::BRACKETS)
				ss << " array of";
			else
				ss << " function returning";
			tokens.pop_front();
		}
	}
}