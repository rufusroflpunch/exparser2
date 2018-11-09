#include <string>
#include <vector>
#include <locale>
#include <sstream>

using namespace std;

using IterType = decltype(string{}.begin());

string get_token(IterType& start, const IterType& end)
{
  string tok{};

  while (start != end)
  {
    while (isspace(*start)) start++; // skip white space
    if (isdigit(*start))
    {
      while (isdigit(*start) || *start == '.')
        tok += *(start++);
      return tok;
    }
    else if (*start == '+' || *start == '-' || *start == '*' || *start == '/')
    {
      tok += *(start++);
      return tok;
    }
    else
    {
      stringstream ss;
      ss << "Invalid token found: " << *start << '\n';
      throw ss.str();
    }
  }
  return tok;
}

vector<string> get_tokens(string input)
{
  auto str_start = begin(input), str_end = end(input);
  vector<string> tokens{};
  auto tok = get_token(str_start, str_end);
  while (tok != "")
  {
    tokens.push_back(tok);
    tok = get_token(str_start, str_end);
  }
  return tokens;
}