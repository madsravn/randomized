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
 *
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
std::vector<int32> randforhash() {
    std::vector<int32> v;
    std::uniform_int_distribution<> dis(0, p-1);
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



int main(int argc, char** argv) {
    // Seeding with now instead of random_device.
    gen.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> dis(0,p-1);


    auto v1 = loadfile("data1a");
    auto v2 = loadfile("data1b");
    auto v3 = loadfile("data2a");
    auto v4 = loadfile("data2b");
    auto v5 = loadfile("data3a");
    auto v6 = loadfile("data3b");
    auto v7 = loadfile("data4a");
    auto v8 = loadfile("data4b");
    auto v9 = loadfile("data5a");
    auto v10 = loadfile("data5b");
    std::cout << "Equal(v1,v2) = " << equal(v1,v2) << std::endl;
    std::cout << "Equal(v3,v4) = " << equal(v3,v4) << std::endl;
    std::cout << "Equal(v5,v6) = " << equal(v5,v6) << std::endl;
    std::cout << "Equal(v7,v8) = " << equal(v7,v8) << std::endl;
    std::cout << "Equal(v9,v10) = " << equal(v9,v10) << std::endl;


    // TODO: HVOR TIT SKAL HASH OG Z VÆLGES? 
    std::vector<int32> hash = randforhash();
    const int z = dis(gen);
    std::cout << "Fingerprint of data1a: " << fingerprint(z, hash, v1) << std::endl;
    std::cout << "Fingerprintstream of data1a: " << fingerprintstream(z, hash, "data1a") << std::endl;
    std::cout << "Fingerprint of data1b: " << fingerprint(z, hash, v2) << std::endl;
    std::cout << "Fingerprint of data2a: " << fingerprint(z, hash, v3) << std::endl;
    std::cout << "Fingerprint of data2b: " << fingerprint(z, hash, v4) << std::endl;
    std::cout << "Fingerprint of data3a: " << fingerprint(z, hash, v5) << std::endl;
    std::cout << "Fingerprint of data3b: " << fingerprint(z, hash, v6) << std::endl;
    std::cout << "Fingerprint of data4a: " << fingerprint(z, hash, v7) << std::endl;
    std::cout << "Fingerprint of data4b: " << fingerprint(z, hash, v8) << std::endl;
    std::cout << "Fingerprint of data5a: " << fingerprint(z, hash, v9) << std::endl;
    std::cout << "Fingerprint of data5b: " << fingerprint(z, hash, v10) << std::endl;





	return 0;
}
