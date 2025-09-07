#ifndef HASHCHAIN_H
#define HASHCHAIN_H

#include <string>
#include <vector>
#include <cryptopp/sha.h>

class HashChain
{
public:
    HashChain(int length);
    
    void generateChain(const std::string& seed);
    std::string getHash(int index) const;
    std::string computeHashN(const std::string& value, int n) const;
    static std::string hashFunction(const std::string& input);
    
    std::string getSeed() const { return h0; }
    std::string getFinalHash() const { return chain.empty() ? "" : chain.back(); }
    
private:
    int chainLength;
    std::vector<std::string> chain;
    std::string h0; // Initial seed
};

#endif // HASHCHAIN_H
