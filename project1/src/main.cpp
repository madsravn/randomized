#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <iterator>
#include <chrono>
#include <cmath>
#include <limits>
#include "Timer.hpp"

typedef unsigned long long uint64;
std::vector<int> currentlist;
std::mt19937 gen;

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
	std::generate_n(vec.begin(), vec.size(), [&](){return el++; });

	std::random_device rd;
	std::mt19937 gen(rd());
	gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<> dis(100, 1000);
	int times = dis(gen);

	for (int i = 0; i < times; ++i) {
		std::shuffle(vec.begin(), vec.end(), gen);
	}
	return vec;
}

int find(std::vector<int> L, int k, uint64& depth, uint64& comparisons) {
	std::vector<int> L1, L2;
	++depth;

	std::uniform_int_distribution<> dis(0, L.size() - 1);

	int pos = dis(gen);
	int e = L.at(pos);
	L.erase(L.begin() + pos);


	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2), // true, false
		[&](int i) {++comparisons; return i < e; });

	if (L1.size() == (k - 1)) {
		return e;
	}
	if (L1.size() >(k - 1)) {
		return find(L1, k, depth, comparisons);
	}
	else {
		return find(L2, k - 1 - L1.size(), depth, comparisons);
	}
}


std::vector<int> quicksort(std::vector<int> L, uint64& depth, uint64& comparisons)
{
	std::vector<int> L1, L2;
	++depth;
	if (L.size() <= 1)
	{
		return L;
	}
	std::uniform_int_distribution<> dis(0, L.size() - 1);
	int pos = dis(gen);
	int  e = L.at(pos);
	L.erase(L.begin() + pos);
	std::partition_copy(std::begin(L), std::end(L), std::back_inserter(L1), std::back_inserter(L2),
		[&](int i){++comparisons; return i < e; });
	L1 = quicksort(L1, depth, comparisons);
	L1.push_back(e);
	L2 = quicksort(L2, depth, comparisons);
	L1.insert(L1.end(), L2.begin(), L2.end());
	return L1;
}

int testingonetwo(const int powerofn) {

	for (int pon = 2; pon < powerofn + 1; ++pon) {
		int n = pow(10, pon);
		currentlist = generate_integers(n);
		std::vector<int> ks{ 1 };
		for (int i = 2; i < 10; ++i) {
			ks.push_back((n / 10)*i);
		}
		for (const auto& k : ks) {

			uint64 comps = 0;
			uint64 depth = 0;
			Timer t;
			t.start();
			find(currentlist, k, depth, comps);
			t.stop();
			std::cout << "n: " << n << " and k: " << k << " took " << t.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;


		}
	}
	return 1;
}

//TODO: ATLEAST N, PREFERABLY MORE
int test_k_runs(const int powerofn, const int runs)
{
	for (int pon = 2; pon < powerofn + 1; ++pon)
	{
		int n = pow(10, pon);
		currentlist = generate_integers(n);
		std::vector<int> ks{ 1 };
		for (int i = 2; i < 10; ++i)
		{
			ks.push_back((n / 10)*i);
		}
		Timer ft;
		Timer qt;

		uint64 lowZ = 0;
		uint64 highZ = 0;

		long double time = 0.0;
		long double lowTime = std::numeric_limits<long double>::max();
		long double highTime = 0.0;

		Timer totalft;
		long double lowfTime= std::numeric_limits<long double>::max();
		long double highfTime = 0.0;
		uint64 leastDepth = std::numeric_limits<long int>::max();
		uint64 maxDepth = 0;
		uint64 leastComps = std::numeric_limits<long int>::max();
		uint64 maxComps = 0;
		uint64 totalComps = 0;
		uint64 totalDepth = 0;
		totalft.start();
		for (int i = 0; i < runs; ++i)
		{
			ft.reset();
			//qt.reset();
			//For each k lists, run through the list for each k.
			for (const auto& k : ks)
			{
				uint64 comps = 0;
				uint64 depth = 0;
				//TODO: Timeren bliver brugt forkert her
				
				ft.start();
				find(currentlist, k, depth, comps);
				ft.stop();
				if(ft.duration().count() >= highfTime) highfTime = ft.duration().count();
				if(ft.duration().count() <= lowfTime) lowfTime = ft.duration().count();
				if(comps> maxComps) maxComps = comps;
				if(comps<leastComps) leastComps = comps;
				if(depth<leastDepth) leastDepth = depth;
				if(depth>maxDepth) maxDepth = depth;
				totalComps += comps;
				totalDepth += depth;
		//		std::cout << "find - n: " << n << " and k: " << k << " took " << ft.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;
			}
				//std::cout << "find - n: " << n  << " took " << ft.duration().count() << " ms." << std::endl;

			uint64 comps = 0;
			uint64 depth = 0;
			
			qt.reset();
			qt.start();
			quicksort(currentlist, depth, comps);
			qt.stop();
			time += qt.duration().count();
			//std::cout << "sort -n: " << n << " took " << qt.duration().count() << " ms and had depth: " << depth << " and comps: " << comps << std::endl;

			if (qt.duration().count() >= highTime) highTime = qt.duration().count();
			if (qt.duration().count() <= lowTime) lowTime = qt.duration().count();
		}
		totalft.stop();
		std::cout << runs << " finds took \t" << totalft.duration().count() << " on n=" << n << "\tAverage time: \t" << totalft.duration().count()/runs << std::endl;
		std::cout << "Fastest time:\t" << lowfTime << "\tSlowest time:\t" << highfTime << std::endl;
		std::cout << "minDepth:\t" << leastDepth << "\tmaxDepth:\t" << maxDepth << "\tAvgDepth:\t" << totalDepth/(10*runs) << std::endl; //Each run uses 10 different values of k
		std::cout << "minComps\t" << leastComps << "\tmaxComps:\t" << maxComps << "\tavgComps\t" << totalComps/(10*runs) << std::endl;
		std::cout << runs << " quicksorts took \t" << time << "ms.\t" << " on n=" << n << "\tAverage time: \t" << (time)/runs << std::endl;
		std::cout << "Fastest time:\t" << lowTime << "\tSlowest time:\t" << highTime << std::endl;
	}
	return 1;
}

int main(int argc, char* argv[]) {

	// Ifølge dokumentationen for random_device er det ikke sikkert den er implementeret ved alle compilere endnu, så vi seeder lige med tiden oveni.	
	gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	if (argc != 3)
	{
		std::cout << "Wrong number of params" << std::endl;
		std::cout << "Usage: randAlg <powers of n> <number of runs>" << std::endl;
		std::cin.ignore();
		return 1;
	}
	int pon = atoi(argv[1]);
	int runs = atoi(argv[2]);
	test_k_runs(pon, runs);
	//std::cin.ignore();
	return 0;
}
