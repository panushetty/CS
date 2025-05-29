/*
Generate RSA public and private keys (choose small prime numbers for simplicity).
Encrypt a message (a small integer) using the public key.
Decrypt the encrypted message using the private key.
Print the original message, encrypted message (ciphertext), and decrypted message.
*/
#include <iostream>
using namespace std;

long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long modInverse(long long e, long long phi) {
    for (long long d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) return d;
    }
    return -1;
}

int main() {
    long long p=3,q=11;
    long long n=p*q;
    long long phi=(p-1)*(q-1);
    long long e=7;
    if(gcd(e,phi)!=1){
        cout<<"e is not coprime with phi(n)"<<endl;
        return 1;
    }

    long long d = modInverse(e, phi);
    if(d==-1){
        cout<<"Failed to find modular inverse for e"<<endl;
        return 1;
    }

    cout << "p: " << p << ", q: " << q << endl;
    cout << "n: " << n << endl;
    cout << "phi(n): " << phi << endl;
    cout << "Public Key (e, n): (" << e << ", " << n << ")" << endl;
    cout << "Private Key (d, n): (" << d << ", " << n << ")" << endl;

    long long message = 5;
    long long ciphertext = modExp(message, e, n);
    long long decrypted = modExp(ciphertext, d, n);


    cout << "Original Message: " << message << endl;
    cout << "Encrypted Message (Ciphertext): " << ciphertext << endl;
    cout << "Decrypted Message: " << decrypted << endl;



    return 0;
}

