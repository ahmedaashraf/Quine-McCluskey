/*
Quine–McCluskey Method for logic minimization
Created by : Ahmed Taha & Mayada El Ghamry Copyright © 2017 All rights reserved .
*/


#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <map>
#include <vector>

using namespace std;

struct minterm
{
	int dec, NoOf1, included;
	string bin;
	vector<int> representation;
	bool dontcare, check, ESPI, crossed;
	minterm()
	{
		dec = 0;
		NoOf1 = 0;
		included = 0;
		bin = "";
		dontcare = false;
		check = false;
		ESPI = false;
		crossed = false;
	}
	bool operator < (const minterm & rhs) const {
		return (NoOf1 < rhs.NoOf1);
	}
};

string bintobool(string x)
{
	string y = "";

	for (int i = 0; i < x.length(); i++)
	{
		if (x[i] == '1')
			y = y + (char)('A' + i);

		else if (x[i] == '0')
			y = y + (char)('A' + i) + "'";

	}

	return y;

}

string dectobin(int num, int NoOFvars)
{

	string binNO = "";
	int Diff;
	if (num == 0)
		binNO += "0";
	while (num != 0)
	{
		if ((num % 2) == 0)
			binNO += "0";
		else
			binNO += "1";
		num /= 2;
	}
	reverse(binNO.begin(), binNO.end());
	Diff = NoOFvars - binNO.length();
	for (int i = 0; i<Diff; i++)
		binNO = "0" + binNO;
	return binNO;
}

int count1(string s)
{
	int c = 0;
	for (int i = 0; i < s.length(); i++)
		if (s[i] == '1')
			c++;
	return c;
}

bool CompareI(vector<minterm>& mt1, vector <minterm>& mt2, int NOofvars)
{
	for (int i = 0; i < mt1.size(); i++)
	{
		for (int j = i + 1; j < mt1.size(); j++)
		{
			string tmp = "";
			int counter = 0;

			if ((mt1[i].NoOf1 - mt1[j].NoOf1))
			{
				if (abs(mt1[i].NoOf1 - mt1[j].NoOf1) == 1)
				{
					counter = 0;
					for (int k = 0; k < NOofvars; k++)
					{
						if (mt1[i].bin[k] == mt1[j].bin[k])
							tmp += mt1[i].bin[k];
						else
						{
							tmp += '-';
							counter++;
						}
					}
					if (counter == 1)
					{
						minterm t;
						t.bin = tmp;
						t.NoOf1 = mt1[i].NoOf1;
						mt1[i].check = true;
						mt1[j].check = true;
						for (int b = 0; b < mt1[i].representation.size(); b++)
							t.representation.push_back(mt1[i].representation[b]);
						for (int b = 0; b < mt1[j].representation.size(); b++)
							t.representation.push_back(mt1[j].representation[b]);
						mt2.push_back(t);
					}
				}
				else break;
			}
		}
	}
	return mt2.size();
}

void CompareII(vector<minterm>& mt, vector<minterm>& Pi)
{

	for (int i = 0; i < mt.size(); i++)
	{

		if (!mt[i].dontcare)
		{
			for (int j = 0; j < Pi.size(); j++)

				for (int g = 0; g < Pi[j].representation.size(); g++)

					if (mt[i].dec == Pi[j].representation[g])

						mt[i].included++;

			bool found = false;


			if (mt[i].included == 1)

				for (int j = 0; j < Pi.size() && !found; j++)

					for (int g = 0; g < Pi[j].representation.size(); g++)

						if (mt[i].dec == Pi[j].representation[g])
						{
							Pi[j].ESPI = true;
							found = true;
						}


		}


	}



}

int exists(vector <minterm> x, int y)
{
	if (x.size())
		for (int i = 0; i < x.size(); i++)
		{
			if (x[i].dec == y)
				return i;
		}

	return -1;
}

int exists(vector <minterm> x, string y)
{
	if (x.size())
		for (int i = 0; i < x.size(); i++)
		{
			if (x[i].bin == y)
				return i;
		}

	return -1;
}

void printfunc(vector <minterm> mt, vector <minterm> Pi)
{
	
	vector <string> tmp;

	for (int i = 0; i < Pi.size(); i++)
	
		if (Pi[i].crossed == true)

			tmp.push_back (bintobool(Pi[i].bin));

	for (int j = 0; j < tmp.size() ; j++)

	{
		cout << tmp[j];

		if ((tmp.size() - j) != 1)
			cout << " + ";


	}

	cout << endl;
			
}

bool isallcrossed(vector <minterm> mt)
{
	for (int i = 0; i < mt.size(); i++)
		if (!mt[i].crossed && !mt[i].dontcare)
			return false;
	return true;
}
bool isallcrossed2(vector <minterm> prime)
{
	for (int i = 0; i < prime.size(); i++)
		if (!prime[i].crossed)
			return false;
	return true;
}
int DomCol(vector<minterm> mt)
{
	int c = 0, a = -1;
	for (int i = 0; i < mt.size(); i++)
	{
		if (!mt[i].crossed)
			if (c < mt[i].included)
			{
				c = mt[i].included;
				a = i;
			}
	}
	return a;
}
int DomRow(int x, vector<minterm> prime, vector<minterm> mt)
{
	int c = 0, max = 0, index = -1;
	bool found = false;
	for (int i = 0; i < prime.size(); i++)
	{
		if (!prime[i].crossed)
		{
			found = false;
			c = 0;
			for (int j = 0; j < prime[i].representation.size(); j++)
				for (int k = 0; k<mt.size(); k++)
				{
					if (prime[i].representation[j] == mt[k].dec && !mt[k].crossed)
						c++;
					if (mt[x].dec == prime[i].representation[j])
						found = true;
				}
		}
		if ((max < c) && found)
		{
			max = c;
			index = i;
		}
	}
	return index;
}
void optimize(vector<minterm> &mt, vector<minterm>&prime)
{
	int c = 0, a = 0;
	for (int i = 0; i < prime.size(); i++)
		if (prime[i].ESPI)
		{
			prime[i].crossed = true;
			for (int j = 0; j < prime[i].representation.size(); j++)
				for (int k = 0; k < mt.size(); k++)
					if (prime[i].representation[j] == mt[k].dec)
					{
						mt[k].crossed = true;
					}
		}
	while (!isallcrossed(mt))
	{
		if (!isallcrossed2(prime))
		{
			c = DomCol(mt);
			if (c != -1)
				a = DomRow(c, prime, mt);
			if (a != -1)
			{
				prime[a].crossed = true;
				for (int i = 0; i < prime[a].representation.size(); i++)
				{
					for (int j = 0; j < mt.size(); j++)
						if (mt[j].dec == prime[a].representation[i])
						{
							mt[j].crossed = true;
						}
				}
			}
		}
	//	for (int l = 0; l < mt.size(); l++)
		//	if (mt[l].crossed)
			//	cout << mt[l].dec << "  ";
		//cout << endl;
	}
}

int main()
{
	int NoOFvars, t;
	minterm tmp;
	cout << "Please Enter the Number of Variables .\n";
	cin >> NoOFvars;

	cout << endl;

	vector <minterm> mt;
	cout << "Please Enter the Minterms, When Finished enter  -1 \n";
	cin >> t;
	if (t == -1)
	{
		cout << "F=0\n";
	}
	else
	{
		while (t != -1)
		{

			if ((t > (pow(2, NoOFvars) - 1)) || (t < 0))
				cout << "Invalid Entry!!!\n";
			else if (exists(mt, t) != -1)
				cout << "Value already entered\n";
			else
			{
				tmp.dec = t;
				tmp.bin = dectobin(t, NoOFvars);
				tmp.NoOf1 = count1(tmp.bin);
				tmp.representation.push_back(t);
				mt.push_back(tmp);
				tmp.representation.pop_back();
			}
			cin >> t;
		}
		cout << endl;

		cout << "Please Enter the Don't cares, When Finished enter  -1. \n";
		cin >> t;

		while (t != -1)
		{
			if ((t > (pow(2, NoOFvars) - 1)) || t < 0)
				cout << "Invalid Entry!!!\n";
			else if (exists(mt, t) != -1)
			{
				int a = exists(mt, t);
				if (!mt[a].dontcare)
				{
					cout << "Value already entered\n";
					char c;
					cout << "Number entered as MINTERM\n" << "If it's a minterm enter m, if it's a don't care enter d \n";
					cin >> c;
					if (c == 'd' || c == 'D')
						mt[a].dontcare = true;
				}
			}
			else
			{
				tmp.dontcare = true;
				tmp.dec = t;
				tmp.bin = dectobin(t, NoOFvars);
				tmp.NoOf1 = count1(tmp.bin);
				tmp.representation.push_back(t);
				mt.push_back(tmp);
				tmp.representation.pop_back();
			}
			cin >> t;
		}

		cout << endl;


		if (mt.size() == pow(2, NoOFvars))
		{
			cout << "F=1\n";
		}


		else
		{
			cout << "Prime Implicants :" << endl;

			sort(mt.begin(), mt.end());
			vector<minterm> mt2;
			vector<minterm> mt1(mt);
			vector <minterm> prime;
			while (CompareI(mt1, mt2, NoOFvars))
			{
				for (int i = 0; i < mt1.size(); i++)
					if (!mt1[i].check && exists(prime, mt1[i].bin) == -1)
						prime.push_back(mt1[i]);
				mt1.clear();
				mt1 = mt2;
				mt2.clear();
			}
			for (int i = 0; i < mt1.size(); i++)
				if (exists(prime, mt1[i].bin) == -1)
					prime.push_back(mt1[i]);
			for (int i = 0; i < prime.size(); i++)
				cout << prime[i].bin << "   " << bintobool(prime[i].bin) << endl;

			cout << endl;

			cout << "Essential Prime Implicants :" << endl;
			CompareII(mt, prime);

			for (int i = 0; i < prime.size(); i++)
				if (prime[i].ESPI == true)
					cout << prime[i].bin << "   " << bintobool(prime[i].bin) << endl;

			cout << endl;

			cout << "Function:" << endl;
			optimize(mt, prime);
			printfunc(mt, prime);
			cout << endl;
		}
	}


	system("pause");
	return NULL;
}





