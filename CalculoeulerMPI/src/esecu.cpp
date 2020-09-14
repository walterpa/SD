#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
int main() {

	unsigned long long fact = 1;
	//const double EPSILON = 1.0e-20;
	double e = 2.0, e0;
	int n = 2, k, i;
	cout << "Ingrese K: ";
	cin >> k;

		for (i = 1; i < k; i++) {
			e0 = e;
			fact *= n++;   //fact =fact * n++
			e += 1.0 / fact;
		};
		//cout << fact << endl;
		//cout << e << endl;
	cout << "e = " << setprecision(20) << e << endl;
	return 0;
};
