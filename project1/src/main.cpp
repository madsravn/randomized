#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <iterator>
#include <chrono>
#include <cmath>
#include "Timer.hpp"

typedef unsigned long long uint64;

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



int find(std::vector<int> L, int k, std::mt19937& gen, uint64& depth, uint64& comparisons) {
	std::vector<int> L1, L2;
	++depth;

	std::uniform_int_distribution<> dis(0, L.size()-1);

	int pos = dis(gen);
	int e = L.at(pos);
	L.erase(L.begin() + pos);


	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2), // true, false
			[&](int i) {++comparisons; return i < e; });

	if (L1.size() == (k - 1)) {
		return e;
	}
	if (L1.size() > (k - 1)) {
		return find(L1, k, gen, depth, comparisons);
	} else {
		return find(L2, k - 1 - L1.size(), gen, depth, comparisons);
	}
}


std::vector<int> quicksort(std::vector<int> L, std::mt19937& gen, uint64& depth, uint64& comparisons)
{
	std::vector<int> L1, L2;
	++depth;
	if(L.size() <= 1)
	{
		return L;
	}
	std::uniform_int_distribution<> dis(0,L.size()-1);
	int pos = dis(gen);
	int  e = L.at(pos);
	L.erase(L.begin() + pos);
	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2), 
			[&](int i){++comparisons; return i<e;});
	L1 = quicksort(L1, gen, depth, comparisons);
        L1.push_back(e);
	L2 = quicksort(L2, gen, depth, comparisons);
	L1.insert(L1.end(), L2.begin(), L2.end());
	return L1;

}

int testingonetwo(const int powerofn) {

    for(int pon = 2; pon < powerofn+1; ++pon) {
        int n = pow(10, pon);
        std::vector<int> ks {1};
        for(int i = 2; i < 10; ++i) {
            ks.push_back((n/10)*i);
        }
        for(const auto& k : ks) {
            std::random_device rd;
            std::mt19937 gen(rd());

            gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
            uint64 comps = 0;
            uint64 depth = 0;
	    Timer gen_t;
	    gen_t.start();
            std::vector<int> integers = generate_integers(n);
	    gen_t.stop();
	    std::cout << "generated " << n << " integers in " << gen_t.duration().count() << " ms" << std::endl;
            Timer t;
            t.start();
            find(integers, k, gen, depth, comps);
            t.stop();
            std::cout << "n: " << n << " and k: " << k << " took " << t.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;


        }
    }
    return 1;
}

int test_k_runs(const int powerofn)
{
	for(int pon = 2; pon < powerofn+1;++pon)
	{
		int n = pow(10,pon);
		std::vector<int> ks{1};
		for(int i=2; i<10; ++i)
		{
			ks.push_back((n/10)*i);
		}
		for(const auto& i : ks)
		{
			//Generate a list of random numbers for each k, test each k in each list, both quicksort and k find.
			Timer gt;
			gt.start();
			std::random_device rd;
			std::mt19937 gen(rd());
			gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
			std::vector<int> integers = generate_integers(n);
			gt.stop();
			std::cout << "generated " << n << " integers in " << gt.duration().count() << " ms" << std::endl;

			//For each k lists, run through the list for each k.
			for(const auto& k : ks)
			{
				uint64 comps = 0;
				uint64 depth = 0;
				Timer ft;
				ft.start();
				find(integers, k, gen, depth, comps);
				ft.stop();
				std::cout << "find - n: " << n << " and k: " << k << " took " << ft.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;
			}
			uint64 comps = 0;
			uint64 depth = 0;
			Timer qt;
			qt.start();
			quicksort(integers,gen, depth, comps);
			qt.stop();
			std::cout << "sort -n: " << n << " took " << qt.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;
		}
	}
	return 1;
}
int main() {


    std::vector<int> integers = generate_integers(100);

    std::random_device rd;
    std::mt19937 gen(rd());
    // Ifølge dokumentationen for random_device er det ikke sikkert den er implementeret ved alle compilere endnu, så vi seeder lige med tiden oveni.
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    int comps = 0;
    int depth = 0;

    //    std::cout << find(integers, integers.size()/3, gen, depth, comps) << std::endl;
    //std::cout << "comps: " << comps << " and depth: " << depth << std::endl;
    comps = depth =0;
    //std::cout << quicksort(integers, gen, depth, comps) << std::endl;
    //std::cout << "comps: " << comps << " and depth: " << depth << std::endl;
    comps = depth = 0;
    //std::cout << find(integers, integers.size()/3, gen, depth, comps) << std::endl;
    //std::cout << "comps: " << comps << " and depth: " << depth << std::endl; 
    std::cout << "hg find:" << std::endl;
//    testingonetwo(8);h
    test_k_runs(4);
    return 0;
}
