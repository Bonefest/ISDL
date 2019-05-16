#include "../vector.h"

#include <iostream>

using namespace std;

int main(){
	Vector2 v1(-4, 3), v2(-2, 4);
	cout << v1.getX() << endl << v1.getY() << endl;
	cout << v2.getX() << endl << v2.getY() << endl << endl;
	Vector2 v3 = v2.normal();
	cout << v3.getX() << endl << v3.getY() << endl << endl;
	cout << v3.len() << endl;
	return 0;
}