#include "json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main() {
	std::ifstream fin("spritesheet.json");
	json j;
	fin >> j;

	for(auto& x : j["frames"].items())
		std::cout << x.key() <<" - " << x.value() << std::endl;
	return 0;
}