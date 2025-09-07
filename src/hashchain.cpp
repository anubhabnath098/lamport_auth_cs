#include "hashchain.h"
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

HashChain::HashChain(int length) : chainLength(length)
{
    chain.reserve(length);
}

void HashChain::generateChain(const std::string& seed)
{
    h0 = seed;
    chain.clear();
    chain.push_back(seed);
    
    for (int i = 1; i <= chainLength; i++) {
        chain.push_back(hashFunction(chain[i-1]));
    }
}

std::string HashChain::getHash(int index) const
{
    if (index >= 0 && index < chain.size()) {
        return chain[index];
    }
    return "";
}

std::string HashChain::computeHashN(const std::string& value, int n) const
{
    std::string result = value;
    for (int i = 0; i < n; i++) {
        result = hashFunction(result);
    }
    return result;
}

std::string HashChain::hashFunction(const std::string& input)
{
    CryptoPP::SHA256 hash;
    std::string digest;
    
    CryptoPP::StringSource ss(input, true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest)
            )
        )
    );
    
    return digest;
}
