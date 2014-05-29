#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <thread>
#include <cstdint>

typedef char16_t character;
typedef std::uint32_t int32;
typedef std::uint64_t int64;

// Guard against weird compiler implementation
static_assert(sizeof(character) == 2, "This data type is not 16 bit");
static_assert(sizeof(int32) == 4, "This data type is not big enough (32 bit)");
static_assert(sizeof(int64) == 8, "This data type is not big enough (64 bit)");

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

/* TESTING 
 * Køre uendeling mange test på hver datastørrelse
 * Kørselshastighed sammenlign med naiv
 * På endnu større datasæt - konkater evt bare filer sammen. (Husk bare samme størrelse)
	 = * RUn with random dataset, worstcase dataset (kun et distinct element) og bestcase dataset (Alle elementer er distincte).
* Run tests with a small p to test guaranteed collisions effect on error porbability.
 * */


std::vector<std::string> loadfile(std::string filename) {
    std::cout << "Loading " << filename << "..." <<  std::endl;
    std::ifstream myfile(filename);
    std::vector<std::string> lines;

    if(myfile.is_open()) {
        for( std::string line; std::getline(myfile, line); ) {
            lines.push_back(line);
        }

    } else {
        std::cout << "File could not be opened." << std::endl;
    }
    return lines;
}

// Non-const-reference
bool equal(std::vector<std::string> v1, std::vector<std::string> v2) {
    if(v1.size() != v2.size()) {
        return false;
    }
    std::cout << "beep boop... sorting..." << std::endl;
    std::sort(v1.begin(), v1.end(), std::greater<std::string>());
    std::sort(v2.begin(), v2.end(), std::greater<std::string>());
	std::cout << "bleep... done sorting... comparing content..." << std::endl;
    for(int i = 0; i < v1.size(); ++i) {
        if(v1.at(i) != v2.at(i)) {
			std::cout << "blarp... mismatch found... does not compute" << std::endl;
            return false;
        }
    }
	std::cout << "blorp... content equal..." << std::endl;
    return true;
}


struct hash {
    std::vector<int> v;
    int operator()(int w, std::vector<int> x) {
        int product = 1;
        for(const auto& e : v) {
            product = product*(w-e);
        }
        return product;
    }
};

const int32 p = 2147483647;

std::uniform_int_distribution<> dis(0,p-1);

std::vector<int32> randforhash() {
    std::vector<int32> v;
    for(int i = 0; i < 80; ++i) {
        v.push_back(dis(gen));
    }
    return v;
}

int32 hs(const std::vector<int32>& hash, const std::string& line) {
    int64 sum = 0;
    for(int i = 0; i < line.size(); ++i) {
        sum = (sum + line.at(i)*hash.at(i)) % p;
    }
    int32 rsum = sum;
    return rsum;
}

int32 fingerprint(int32 z, const std::vector<int32>& hash, const std::vector<std::string>& lines) {
    int64 product = 1;
    for(const auto& line : lines) {
        product = product * ( z - hs(hash, line) ) % p;
    }
    return product;
}

int32 fingerprintstream(int32 z, const std::vector<int32>& hash, const std::string& filename) {
    int64 product = 1;

    std::ifstream myfile(filename);
    std::vector<std::string> lines;

    if(myfile.is_open()) {
        for( std::string line; std::getline(myfile, line); ) {
            product = product * ( z - hs(hash, line) ) % p;
        }

    } else {
        std::cout << "File could not be opened." << std::endl;
    }
    return product;
}

/* *
 * Returns true if the two sets are determined equal. false if not.
 */
int testStreaming(int n) {
    std::vector<int32> hash = randforhash();
    const int z = dis(gen);
    
    int32 a = fingerprintstream(z, hash, "data" + std::to_string(n+1) + "a");
    int32 b = fingerprintstream(z, hash, "data" + std::to_string(n+1) + "b");

    return (a == b);
}

/* *
 * Returns true if the two sets are determined equal. false if not.
 */
bool testReading(int n) {
    std::vector<int32> hash = randforhash();
    const int z = dis(gen);

    auto v1 = loadfile("data" + std::to_string(n+1) + "a");
    auto v2 = loadfile("data" + std::to_string(n+1) + "b");

    int32 a = fingerprint(z, hash, v1);
    int32 b = fingerprint(z, hash, v2);

    return (a == b);
}

/* *
 * Returns true if the two sets are determined equal. false if not.
 */
bool testReading(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    std::vector<int32> hash = randforhash();
    const int z = dis(gen);

    int32 a = fingerprint(z, hash, v1);
    int32 b = fingerprint(z, hash, v2);

    return (a == b);
}

/* *
 * Returns true the two sets are determined equal. false if not.
 */
bool testDeterministic(int n) {

    auto v1 = loadfile("data" + std::to_string(n+1) + "a");
    auto v2 = loadfile("data" + std::to_string(n+1) + "b");

    return equal(v1,v2);
}

std::string filenamefromnumber(int i) {

    return "data" + std::to_string((i/2)) + (i%2 == 0 ? "a" : "b");
}

int countzeros(int n) {
    int ret = 0;
    for(auto a : std::to_string(n)) {
        if(a == '0') {
            ++ret;
        }
    }
    return ret;
}


void testingThread(const std::vector<std::vector<std::string>>& files, std::vector<bool> dets) {
    std::cout << "Starting thread " << std::this_thread::get_id() << std::endl;

    int sets = 6;

    std::vector<int> errors;
	for (int i = 0; i < sets; ++i) {
        errors.push_back(0);
    }

    const int roundabout = 100;
    int count = 0;
    std::cout << "Let's start this!" << std::endl;
    std::cout << "With dets: " << dets << std::endl;
    while(true) {
		for (int i = 0; i < sets; ++i) {
            for(int j = 0; j < roundabout; ++j) {
                //std::cout << "Testing data" << countzeros(files.at(2*i).size())  << "{a|b}" << std::endl;
                if(testReading(files.at(2*i), files.at(2*i+1)) != dets.at(i)) {
                    errors.at(i)++;
                }
            }
        }
        count+=roundabout;
        std::cout << std::this_thread::get_id() << " count = " << count << std::endl;
        std::cout << std::this_thread::get_id() << " " << errors << std::endl;
    }
}

int main(int argc, char** argv) {

	int sets = 6;
    std::cout << std::thread::hardware_concurrency() << std::endl;

    // Seeding with now instead of random_device.
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Pre-calculations
    std::cout << "Pre-calculating deterministic solutions..." << std::endl;
    std::vector<bool> dets;
	for (int i = 0; i < sets; ++i) {
        dets.push_back(testDeterministic(i));
    }
    std::cout << "Done with deterministic... Proceeding to loading files..." << std::endl;

    
    std::vector<std::vector<std::string>> files;
    for(int i = 2; i < sets*2+2; ++i) {
        files.push_back(loadfile(filenamefromnumber(i)));
    }

    std::thread first {testingThread, ref(files), dets};
    std::thread second {testingThread, ref(files), dets};
    std::thread third {testingThread, ref(files), dets};
    std::thread fourth {testingThread, ref(files), dets};
    first.join();
    second.join();
    third.join();
    fourth.join();

    /*

    std::vector<int> errors;
	for (int i = 0; i < sets; ++i) {
        errors.push_back(0);
    }
    std::cout << "Done pre-loading files" << std::endl;

    // TODO: Skal vi tjekke dets.at(i) == streaming eller streaming == reading? 
    
    const int roundabout = 100;
    int count = 0;
    std::cout << "Let's start this!" << std::endl;
    std::cout << "With dets: " << dets << std::endl;
    while(true) {
		for (int i = 0; i < sets; ++i) {
            for(int j = 0; j < roundabout; ++j) {
                //std::cout << "Testing data" << countzeros(files.at(2*i).size())  << "{a|b}" << std::endl;
                if(testReading(files.at(2*i), files.at(2*i+1)) != dets.at(i)) {
                    errors.at(i)++;
                }
            }
        }
        count+=roundabout;
        std::cout << "count = " << count << std::endl;
        std::cout << errors << std::endl;
    }
    */
            
	return 0;
}
