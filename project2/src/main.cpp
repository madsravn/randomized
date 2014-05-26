#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>

typedef char16_t character;
typedef unsigned long int32;
typedef unsigned long long int64;

// Guard against weird compiler implementation
static_assert(sizeof(character) == 2, "This data type is not 16 bit");
static_assert(sizeof(int32) > 3, "This data type is not big enough (32 bit)");
static_assert(sizeof(int64) > 7, "This data type is not big enough (64 bit)");

std::mt19937 gen;


/* TESTING 
 * Køre uendeling mange test på hver datastørrelse
 * Kørselshastighed sammenlign med naiv
 * På endnu større datasæt - konkater evt bare filer sammen. (Husk bare samme størrelse)
	 = * RUn with random dataset, worstcase dataset (kun et distinct element) og bestcase dataset (Alle elementer er distincte).
* Run tests with a small p to test guaranteed collisions effect on error porbability.
 * */


std::vector<std::string> loadfile(std::string filename) {
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
    std::sort(v1.begin(), v1.end(), std::greater<std::string>());
    std::sort(v2.begin(), v2.end(), std::greater<std::string>());
    for(int i = 0; i < v1.size(); ++i) {
        if(v1.at(i) != v2.at(i)) {
            return false;
        }
    }
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


//TODO: int er sikkert en forkert data type - placeholder indtil videre
/*
std::function<int(int, std::vector<int>)> hashfunction() {
    std::vector<int> v;
    // placeholder
    std::uniform_int_distribution<> dis(0,300);
    const length = 80;
    for(int i = 0; i < length; ++i) {
        v.push_back(dis(gen));
    }

    hash h1;
    h1.v = v;
    return h1;

}
*/

const int32 p = 2147483647;

std::uniform_int_distribution<> dis(0,p-1);

std::vector<int32> randforhash() {
    std::vector<int32> v;
    for(int i = 0; i < 80; ++i) {
        v.push_back(dis(gen));
    }
    return v;
}

int32 hs(std::vector<int32> hash, std::string line) {
    int64 sum = 0;
    for(int i = 0; i < line.size(); ++i) {
        sum = (sum + line.at(i)*hash.at(i)) % p;
    }
    int32 rsum = sum;
    return rsum;
}

int32 fingerprint(int32 z, std::vector<int32> hash, std::vector<std::string> lines) {
    int64 product = 1;
    for(const auto& line : lines) {
        product = product * ( z - hs(hash, line) ) % p;
    }
    return product;
}

int32 fingerprintstream(int32 z, std::vector<int32> hash, std::string filename) {
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

    
    int32 a = fingerprintstream(z, hash, "data" + std::to_string(n) + "a");
    int32 b = fingerprintstream(z, hash, "data" + std::to_string(n) + "b");
    return (a == b);
}

/* *
 * Returns true if the two sets are determined equal. false if not.
 */
int testReading(int n) {
    std::vector<int32> hash = randforhash();
    const int z = dis(gen);

    auto v1 = loadfile("data" + std::to_string(n) + "a");
    auto v2 = loadfile("data" + std::to_string(n) + "b");

    int32 a = fingerprint(z, hash, v1);
    int32 b = fingerprint(z, hash, v2);

    return (a == b);
}

/* *
 * Returns true the two sets are determined equal. false if not.
 */
int testDeterministic(int n) {

    auto v1 = loadfile("data" + std::to_string(n) + "a");
    auto v2 = loadfile("data" + std::to_string(n) + "b");

    return equal(v1,v2);
}



 


int main(int argc, char** argv) {
    // Seeding with now instead of random_device.
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    
    for(int i = 1; i < 8; ++i) {
        bool det = testDeterministic(i);
        int count = 0;
        for(int j = 0; j < 100000; ++j) {
            if(testStreaming(i) != det) {
                ++count;
            }
        }
        std::cout << "At i = " << i << " we encounter an error " << count << " times." << std::endl;
    }
            
	return 0;
}
