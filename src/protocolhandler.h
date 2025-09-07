#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QString>
#include <memory>
#include "hashchain.h"

class ProtocolHandler
{
public:
    ProtocolHandler(bool isAlice, int chainLength);
    
    // Alice functions
    QString generateChallenge(int counter);
    bool verifyResponse(const QString& response);
    
    // Bob functions
    QString generateResponse(const QString& challenge);
    
private:
    bool isAliceRole;
    int chainLength;
    int currentCounter;
    
    std::unique_ptr<HashChain> hashChain;
    std::string h0; // Bob's seed
    std::string hn; // Alice's final hash
    std::string lastValidHash; // For Alice's verification
};

#endif // PROTOCOLHANDLER_H