#include "ShuntingYard.h"

vector<string> ShuntingYard::Parsing_String(string_view in)
{
	vector<string> vector_temp;
	string temp_buf;

	auto it = in.begin();
	while (it != in.end())
	{
		switch (*it)
		{
		case '*':
		case '/':
		case '-':
		case '+':
		case '(':
		case ')':
			if (temp_buf.size() > 0)
			{
				vector_temp.push_back(temp_buf);
				temp_buf.clear();
			}
			vector_temp.push_back(string(1, *it));
			++it;
			break;
		default:
			if (isdigit(*it) || *it == '.')
			{
				temp_buf.push_back(*it);
			}
			if (++it != in.end()) continue;
		}
		if (temp_buf.size() > 0)
		{
			vector_temp.push_back(temp_buf);
			temp_buf.clear();
			if (it != in.end())
			{
				vector_temp.push_back(string(1, *it));
				++it;
			}
		}
	}
	return vector_temp;
}
//------------------------------
queue<string> ShuntingYard::Infix_to_Postfix(string_view in)
{
	queue<string> queue_result;
	stack<string> stack_temp;

	auto vector_temp = Parsing_String(in);

	auto while_pop
	{
		[&]()
		{
			while (!stack_temp.empty()) {
				   queue_result.emplace(stack_temp.top());
				   stack_temp.pop();
			   }
		}
	};

	auto To_postfix
	{
		[&](auto& it)
		{
		if (!stack_temp.empty())
		{
		   if (Get_Priority(stack_temp.top()) < Get_Priority(it))
		   {
			 stack_temp.emplace(it);
		   }
		   else {
			   while_pop();
			   stack_temp.emplace(it);
		   }
		 }
		else stack_temp.emplace(it);
		}
	};

	auto posf
	{
	[&](auto& it)
		{
		  if (it == "*" || it == "/" || it == "-" || it == "+")
		  {
			To_postfix(it);
		  }
		  else {
			 queue_result.emplace(it);
			}

		}
	};

	/* Without brackets
	for (auto& it : vector_temp)
	{
	 posf(it);
	}
	*/

	for (int i = 0; i < vector_temp.size(); i++)
	{
		auto v_i = vector_temp[i];
		if (v_i == "(") {
			stack_temp.push("(");
		}
		else
			if (v_i == ")") {
				while (stack_temp.top() != "(")
				{
					queue_result.emplace(stack_temp.top());
					stack_temp.pop();
				}
				stack_temp.pop();
			}
			else {
				posf(v_i);
			}
	}
	while_pop();
	return queue_result;
}
//-------------------------------------------------
double ShuntingYard::Ñalcul_Postfix(queue<string>& qu)
{
	stack <double> result;
	while (!qu.empty())
	{
		auto temp_v = qu.front();
		if (isdigit(*temp_v.data())) {
			result.emplace(stod(temp_v));
		}
		else {
			auto v1 = result.top();
			result.pop();
			auto v2 = result.top();
			result.pop();
			auto temp_oper = qu.front();
			if (temp_oper == "*") result.emplace(v2 * v1);
			else
				if (temp_oper == "/") result.emplace(v2 / v1);
				else
					if (temp_oper == "+") result.emplace(v2 + v1);
					else
						if (temp_oper == "-") result.emplace(v2 - v1);
		}
		qu.pop();
	}
	return result.top();
}
double ShuntingYard::Calculation(string_view in)
{
	auto queue_result = Infix_to_Postfix(in);
	return Ñalcul_Postfix(queue_result);
}
//-----------------------------
int ShuntingYard::Get_Priority(string_view tk)
{
	if (tk == "+" || tk == "-") return 1;
	else
		if (tk == "*" || tk == "/") return 2;
		else return 0;
}
//=========================Another variation, only without the brackets!!!!=============================================================
vector<string> ShuntingYard_2::Parsing_String(string_view in)
{
	vector<string> vector_temp;
	string temp_buf;

	auto it = in.begin();
	while (it != in.end())
	{
		switch (*it)
		{
		case '*':
		case '/':
		case '-':
		case '+':
		case '(':
		case ')':
			if (temp_buf.size() > 0)
			{
				vector_temp.push_back(temp_buf);
				temp_buf.clear();
			}
			vector_temp.push_back(string(1, *it));
			++it;
			break;
		default:
			if (isdigit(*it) || *it == '.')
			{
				temp_buf.push_back(*it);
			}

			if (++it != in.end()) continue;
		}
		if (temp_buf.size() > 0)
		{
			vector_temp.push_back(temp_buf);
			temp_buf.clear();
			if (it != in.end())
			{
				vector_temp.push_back(string(1, *it));
				++it;
			}
		}
	}
	return vector_temp;
}
//--------------------------
shared_ptr<ShuntingYard_2::ICalc> ShuntingYard_2::Preparation(vector<string>& in)
{
	auto token_carrent = in.begin();
	auto pos_end = in.end();

	if (token_carrent == pos_end) return make_shared<Value>('0');

	stack<shared_ptr<ICalc>> values;
	stack<shared_ptr<MathOperation>> mop;

	auto MPopOps
	{
		[&values, &mop](int pr)
		{
			while (!mop.empty() && mop.top()->priority >= pr)
			{
				auto temp_right = values.top();
				values.pop();
				auto temp_left = values.top();
				values.pop();

				auto temp_oper_plus_digit = mop.top();
				mop.pop();

				temp_oper_plus_digit->SetRight(temp_right);
				temp_oper_plus_digit->SetLeft(temp_left);

				values.push(temp_oper_plus_digit);
			}
		}
	};


	while (token_carrent != pos_end)
	{
		const auto& token = *token_carrent;

		if (token == "*" || token == "/")
		{
			MPopOps(2);
			mop.push(make_shared<MathOperation>(token));
		}
		else if (token == "+" || token == "-")
		{
			MPopOps(1);
			mop.push(make_shared<MathOperation>(token));
		}
		else {
			values.push(make_shared<Value>(stod(token.c_str())));
		}
		++token_carrent;
	}
	while (!mop.empty())
	{
		MPopOps(0);
	}
	return values.top();
}
//---------------------------------------------
double ShuntingYard_2::Calculation(string_view in)
{
	auto res1 = Parsing_String(in);
	auto res2 = Preparation(res1);
	return res2->Calculation();
}
