#include "../vector.h"

#include <iostream>

using namespace std;

int main(){
	Vector2 v1(0, -3);
	cout << v1.isNull() << endl;
	Vector2 v2(0, 0);
	cout << v2.isNull() << endl;
	return 0;
}