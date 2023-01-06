#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cmath>

using namespace std;

vector<string> split(string s, string sep) {
	vector<string> tab;
	size_t i = 0;
	while ((i = s.find_first_of(sep, i+1)) != string::npos) {
		tab.push_back(s.substr(0, i));
		s.erase(0, i);
		i = 0;
	}
	tab.push_back(s);
	return tab;
}

struct Term {

	float coef;
	int degree;

	Term(float _coef = 0, int _degree = 0) : coef(_coef), degree(_degree) {}
	Term(const Term & src) : coef(src.coef), degree(src.degree) {}
	Term &operator=(const Term & src) {
		this->coef = src.coef;
		this->degree = src.degree;
		return *this;
	}

	Term opposite() {
		return Term(-coef, degree);
	}

	string str() {
		string str = to_string(coef) + " * X^" + to_string(degree);
		size_t i = str.find('.');
		if (i != string::npos) {
			while (++i < str.size() && str[i] != '0');
			if (i != str.size()) {
				size_t j = str.find_first_not_of('0', i);
				str.erase(i, j - i);
			}
		}
		i = str.find('.');
		if (i != string::npos && str[i + 1] == ' ')
			str.erase(i, 1);
		return str;
	}
};

struct Polynomial {

	vector<Term> terms;
	int degree;
	string reduceForm;
	const int max_solving_degree = 2;

	Polynomial(string eq = "") : terms(vector<Term>()), degree(0), reduceForm("") {
		if (eq != "") parseEquation(eq);
	}
	Polynomial(const Polynomial & src) : terms(src.terms), degree(src.degree), reduceForm(src.reduceForm) {}
	Polynomial &operator=(const Polynomial & src) {
		this->terms = src.terms;
		this->degree = src.degree;
		this->reduceForm = src.reduceForm;
		return *this;
	}

	int parseEquation(string eq) {
		eq.erase(remove_if(eq.begin(), eq.end(), ::isspace), eq.end());
		vector<string> sides = split(eq, "=");
		sides[1].erase(0, 1);
		vector<string> lhs_terms_str = split(sides[0], "+-");
		vector<string> rhs_terms_str = split(sides[1], "+-");
		vector<Term> lhs_terms;
		vector<Term> rhs_terms;

		// parse equation to create terms
		for (string &str : lhs_terms_str) {
			float coef;
			int degree;
			sscanf(str.c_str(), "%f*X^%d", &coef, &degree);
			lhs_terms.push_back(Term(coef, degree));
		}
		for (string &str : rhs_terms_str) {
			int coef, degree;
			sscanf(str.c_str(), "%d * X^%d", &coef, &degree);
			rhs_terms.push_back(Term(coef, degree));
		}

		// swap right terms to the left
		for (Term &rhs_t : rhs_terms) {
			auto it = find_if(lhs_terms.begin(), lhs_terms.end(), [rhs_t](Term &lhs_t) { return lhs_t.degree == rhs_t.degree; });
			if (it != lhs_terms.end()) {
				it->coef -= rhs_t.coef;
			} else {
				lhs_terms.push_back(rhs_t.opposite());
			}
		}
		terms = lhs_terms;

		// compute polynomial degree
		for (Term &t : terms) degree = max(degree, t.degree);

		computeReduceForm();

		// add missing degree to help solving
		for (int i = 0; i < max_solving_degree + 1; i++) {
			if (find_if(terms.begin(), terms.end(), [i](Term &t) { return t.degree == i; }) == terms.end()) {
				terms.push_back(Term(0, i));
			}
		}

		return 0;
	}

	void computeReduceForm() {
		for (size_t i = 0; i < terms.size(); i++) {
			string to_add = terms[i].str() + " ";
			if (i != 0) {
				if (to_add[0] == '-')
					to_add.insert(1, " ");
				else
					to_add.insert(0, "+ ");
			}
			reduceForm += to_add;
		}
		reduceForm += "= 0";
	}

	void solve() {

		float discriminant, a, b, c, s1, s2;
		if (degree == 0) {

			if (terms[0].coef == 0)
				cout << "There an infinit number of solution." << endl;
			else
				cout << "There is no solution." << endl;

		} else if (degree == 1) {

			cout << "The solution is:" << endl;
			cout << -terms[0].coef / terms[1].coef << endl;

		} else if (degree == 2) {

			a = terms[2].coef, b = terms[1].coef, c = terms[0].coef;
			discriminant = pow(b, 2) - 4 * a * c;
			if (discriminant > 0) {
				s1 = (-b + sqrt(discriminant)) / (2 * a);
				s2 = (-b - sqrt(discriminant)) / (2 * a);
				cout << "Discriminant is strictly negative, the solutions are:" << endl;
				cout << s1 << endl;
				cout << s2 << endl;
			} else if (discriminant == 0) {
				s1 = -b / (2 * a);
				cout << "Discriminant is equal to zero, the solution is:" << endl;
				cout << s1 << endl;
			} else {
				cout << "Discriminant is strictly negative, there is no solution in R." << endl;
			}

		} else {
			cout << "Sorry I can only solve polynomial degree up to " << max_solving_degree << "." << endl;
		}
	}

};

int main(int ac, char *av[]) {

	if (ac != 2) {
		cout << "Error: wrong number of argument" << endl;
		return 1;
	}

	Polynomial p(av[1]);

	cout << "Reduced form: " << p.reduceForm << endl;
	cout << "Polynomial degree: " << p.degree << endl;
	p.solve();

	return 0;
}