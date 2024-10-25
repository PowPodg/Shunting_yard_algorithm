#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <stack>
#include <queue>
#include <sstream>

using namespace std;

class ShuntingYard
{
public:
	vector<string> Parsing_String(string_view);
	queue<string> Infix_to_Postfix(string_view);
	double Ñalcul_Postfix(queue<string>&);
	double Calculation(string_view);
private:
	int Get_Priority(string_view);
};
//============================================================================================
//-----------------------Another variation, only without the brackets!!!!!!----------------------

class ShuntingYard_2
{
private:
	class ICalc {
	public:
		double virtual Calculation() = 0;
	};
public:
	vector<string> Parsing_String(string_view);
	shared_ptr<ICalc> Preparation(vector<string>&);
	double Calculation(string_view in);
private:
	//---------
	class Value :public ICalc
	{
	public:
		Value(double digit) : _value(digit) {}

		double Calculation()  override {
			return _value;
		}
	private:
		const double _value;
	};
	//-----------
	class MathOperation : public ICalc
	{
	public:
		MathOperation(string value)
			: priority
			(
				[value]()
				{
					if (value == "*" || value == "/") return 2;
					else return 1;
				}()
					), math_op(value) {}

				const int priority;
				double Calculation() override {
					if (math_op == "*") {
						auto res = _left->Calculation() * _right->Calculation();
						return res;
					}
					else if (math_op == "/") {
						auto res1 = _left->Calculation();
						auto res2 = _right->Calculation();
						auto res = res1 / res2;
						return res;
					}
					else if (math_op == "+") {
						auto res1 = _left->Calculation();
						auto res2 = _right->Calculation();
						auto res = res1 + res2;
						return res;
					}
					else if (math_op == "-") {
						auto res = _left->Calculation() - _right->Calculation();
						return res;
					}
					return 0;
				}
				void SetLeft(shared_ptr<ICalc> calc) { _left = calc; }
				void SetRight(shared_ptr<ICalc> calc) { _right = calc; }
	private:
		const string math_op;
		shared_ptr<ICalc> _left, _right;
	};
	//----------------------------------------------

};

