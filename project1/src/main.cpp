#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <iterator>
#include <chrono>

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

std::vector<int> generate_integers(int size) {
    std::vector<int> vec(size);
    int el = 1; 
    std::generate_n(vec.begin(), vec.size(), [&](){return el++;});
    
    std::random_device rd;
	std::mt19937 gen(rd());
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> dis(100, 1000);
    int times = dis(gen);

    for(int i = 0; i < times; ++i) {
        std::shuffle(vec.begin(), vec.end(), gen);
    }
    return vec;
}



int find(std::vector<int> L, int k, std::mt19937 gen) {
	std::vector<int> L1, L2;

	std::uniform_int_distribution<> dis(0, L.size()-1);

    int pos = dis(gen);
	int e = L.at(pos);
    L.erase(L.begin() + pos);


	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2), // true, false
		[e](int i) {return i < e; });

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


    std::vector<int> integers = generate_integers(100);

	std::random_device rd;
	std::mt19937 gen(rd());
    // Ifølge dokumentationen for random_device er det ikke sikkert den er implementeret ved alle compilere endnu, så vi seeder lige med tiden oveni.
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	std::cout << find(integers, integers.size()/3, gen) << std::endl;

	return 0;
}
