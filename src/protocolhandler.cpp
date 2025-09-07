#include "protocolhandler.h"
#include <QDebug>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

ProtocolHandler::ProtocolHandler(bool isAlice, int chainLength)
    : isAliceRole(isAlice)
    , chainLength(chainLength)
    , currentCounter(1)
{
    hashChain = std::make_unique<HashChain>(chainLength);
    
    // Generate or load pre-stored values
    if (!isAlice) {
        // Bob: Generate chain from seed
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::byte seed[32];
        rng.GenerateBlock(seed, 32);
        
        std::string seedStr;
        CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(seedStr));
        encoder.Put(seed, 32);
        encoder.MessageEnd();
        
        hashChain->generateChain(seedStr);
        h0 = hashChain->getSeed();
        hn = hashChain->getFinalHash();
        
        qDebug() << "Bob's h0:" << QString::fromStdString(h0);
        qDebug() << "Bob's hn:" << QString::fromStdString(hn);
    } else {
        // Alice: Use pre-computed hn (in real implementation, this would be securely transferred)
        // For demo, we use a fixed chain
        std::string demoSeed = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF";
        hashChain->generateChain(demoSeed);
        hn = hashChain->getFinalHash();
        lastValidHash = hn;
        
        qDebug() << "Alice's hn:" << QString::fromStdString(hn);
    }
}

QString ProtocolHandler::generateChallenge(int counter)
{
    currentCounter = counter;
    return QString::number(counter);
}

QString ProtocolHandler::generateResponse(const QString& challenge)
{
    int c = challenge.toInt();
    
    // For demo purposes, use the pre-computed chain
    if (!isAliceRole) {
        // In real implementation, Bob would compute H^(n-c)(h0)
        std::string demoSeed = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF";
        HashChain tempChain(chainLength);
        tempChain.generateChain(demoSeed);
        
        // r = H^(n-c)(h0)
        std::string response = tempChain.computeHashN(tempChain.getSeed(), chainLength - c);
        return QString::fromStdString(response);
    }
    
    return "";
}

bool ProtocolHandler::verifyResponse(const QString& response)
{
    if (isAliceRole) {
        std::string r = response.toStdString();
        std::string hashedResponse = HashChain::hashFunction(r);
        
        bool valid = (hashedResponse == lastValidHash);
        
        if (valid) {
            lastValidHash = r; // Update for next verification
        }
        
        return valid;
    }
    
    return false;
}
