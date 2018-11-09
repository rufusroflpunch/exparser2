#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <memory>
#include <stack>

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

struct Node {
  virtual ~Node() {}
  virtual string pprint() { return string{"Dummy string"}; };
};
using NodePtr = unique_ptr<Node>;
struct OpNode: public Node
{
  string op;
  OpNode(string op) : op(op) {}
  string pprint() override { return op; }
};
struct NumNode : public Node
{
  double value;
  NumNode(double value) : value(value) {}
  string pprint() override {
    stringstream ss;
    ss << value;
    return ss.str();
  }
};
struct BinOpNode : public Node
{
  NodePtr op, lhs, rhs;
  BinOpNode(NodePtr op, NodePtr lhs, NodePtr rhs) : op(move(op)), lhs(move(lhs)), rhs(move(rhs)) {}
  string pprint() override {
    stringstream ss;
    ss << "(" << lhs->pprint() << " " << op->pprint() << " " << rhs->pprint() << ")";
    return ss.str();
  }
};

enum class PState
{
  Done,
  Expression,
  Num,
  Add,
  AddOp,
  MakeBinOp
};

NodePtr parse(const vector<string>& tokens)
{
  auto st = stack<PState>{};
  auto node_st = stack<NodePtr>{};
  st.push(PState::Done);
  st.push(PState::Expression);

  auto tok_start = begin(tokens);
  while (st.top() != PState::Done)
  {
    switch(st.top())
    {
      case PState::Done:
        throw string{"Parse error: Incomplete parse"};
      case PState::Expression:
        cerr << "Parsing Expression: " << *tok_start << '\n';
        st.pop();
        st.push(PState::Add);
        break;
      case PState::Add:
        cerr << "Parsing Add: " << *tok_start << '\n';
        st.pop();
        st.push(PState::MakeBinOp);
        st.push(PState::Num);
        st.push(PState::AddOp);
        st.push(PState::Num);
        break;
      case PState::AddOp:
        cerr << "Parsing AddOp: " << *tok_start << '\n';
        if (*tok_start != "+" && *tok_start != "-") throw string{"Unexpected token: Expected add op"};
        node_st.push(make_unique<OpNode>(*tok_start));
        st.pop();
        tok_start++;
        break;
      case PState::Num:
        cerr << "Parsing Num: " << *tok_start << '\n';
        st.pop();
        try {
          double v = stod(*tok_start);
          node_st.push(make_unique<NumNode>(v));
          tok_start++;
        } catch(std::exception) {
          throw string{"Unexpected token: expected number"};
        }
        break;
      case PState::MakeBinOp:
        cerr << "Parsing MakeBinOp: " << *tok_start << '\n';
        st.pop();
        if (node_st.size() != 3) throw string{"Parse error: expected three nodes for binary operation"};
        NodePtr rhs = move(node_st.top());
        node_st.pop();
        NodePtr op = move(node_st.top());
        node_st.pop();
        NodePtr lhs = move(node_st.top());
        node_st.pop();
        node_st.push(make_unique<BinOpNode>(move(op), move(lhs), move(rhs)));
        break;
    }
  }
  return move(node_st.top());
}