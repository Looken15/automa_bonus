#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void remove_chain_rules(string filename)
{
	ifstream file(filename);
	string gramm;
	getline(file, gramm);

	vector<string> rules = split(gramm, " ");

	set<string> Term;
	set<string> pTerm;
	set<string> pnTerm;

	for (auto x : rules)
	{
		for (auto i = 0; i < x.size(); ++i)
		{
			if (isupper(x[i]))
				Term.insert(string(1, x[i]));
		}
	}

	int a = 0;
	while (a != rules.size())
	{
		if (a == 0)
		{
			for (auto i = 0; i < rules.size(); ++i)
			{
				auto split_rule_a = split(rules[i], "->");
				auto split_rule = split(split_rule_a[1], "|");
				for (auto k = 0; k < split_rule.size(); ++k)
				{
					for (auto j = 0; j < split_rule[k].size(); ++j)
					{
						auto z = split_rule[k][j];
						if (j == split_rule[k].size() - 1 && isupper(z))
						{
							pTerm.insert(split_rule_a[0]);
							break;
						}
						else if (islower(z))
							break;
					}
				}
			}
		}
		else
		{
			for (auto i = 0; i < rules.size(); ++i)
			{
				auto split_rule_a = split(rules[i], "->");
				auto split_rule = split(split_rule_a[1], "|");
				for (auto k = 0; k < split_rule.size(); ++k)
				{
					for (auto j = 0; j < split_rule[k].size(); ++j)
					{
						auto z = split_rule[k][j];
						if (pTerm.find(string(1, z)) == pTerm.end() && isupper(z))
							break;
						if (j == split_rule[k].size() - 1)
							pTerm.insert(split_rule_a[0]);
					}
				}
			}
		}
		a++;
	}

	set_difference(Term.begin(), Term.end(), pTerm.begin(), pTerm.end(), inserter(pnTerm, pnTerm.begin()));

	for (auto i = 0; i < rules.size(); ++i)
	{
		for (auto y : pnTerm)
		{
			if (rules[i].find(y) != string::npos)
			{
				rules.erase(remove(rules.begin(), rules.end(), rules[i]), rules.end());
				i = 0;
			}
		}
	}


	for (auto x : rules)
	{
		cout << x << endl;
	}
}

int main()
{
	remove_chain_rules("example.txt");
}
