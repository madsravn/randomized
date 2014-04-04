#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <iterator>

template <class T>
inline std::ostream& operator << (std::ostream& os, const std::vector<T>& v) 
{
    os << "[";
    for (const auto& elem : v)
    {
        os << " " << elem;
    }
    os << " ]";
    return os;
}

int find(std::vector<int> L, int k, std::mt19937 gen) {
	std::vector<int> L1, L2;

	std::uniform_int_distribution<> dis(0, L.size()-1);

    int pos = dis(gen);
	int e = L.at(pos);
    L.erase(L.begin() + pos);


	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2), // true, false
		[&](int i) {return i > e; });

	if (L1.size() == (k - 1)) {
		return e;
	}
	if (L1.size() > (k - 1)) {
		return find(L1, k, gen);
	} else {
		return find(L2, k - 1 - L1.size(), gen);
	}
	
}

int main() {


	std::vector<int> integers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	std::random_device rd;
	std::mt19937 gen(rd());

	std::cout << find(integers, 4, gen) << std::endl;

	return 0;
}
