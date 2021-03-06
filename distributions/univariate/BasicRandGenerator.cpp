#include "BasicRandGenerator.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <thread>

unsigned long RandEngine::mix(unsigned long a, unsigned long b, unsigned long c)
{
    a = a - b;  a = a - c;  a = a ^ (c >> 13);
    b = b - c;  b = b - a;  b = b ^ (a << 8);
    c = c - a;  c = c - b;  c = c ^ (b >> 13);
    a = a - b;  a = a - c;  a = a ^ (c >> 12);
    b = b - c;  b = b - a;  b = b ^ (a << 16);
    c = c - a;  c = c - b;  c = c ^ (b >> 5);
    a = a - b;  a = a - c;  a = a ^ (c >> 3);
    b = b - c;  b = b - a;  b = b ^ (a << 10);
    c = c - a;  c = c - b;  c = c ^ (b >> 15);
    return c;
}

unsigned long RandEngine::getRandomSeed()
{
    static thread_local unsigned long dummy = 123456789;
    return mix(time(0), std::hash<std::thread::id>()(std::this_thread::get_id()), ++dummy);
}

void JKissRandEngine::Reseed(unsigned long seed)
{
    X = 123456789 ^ seed;
    C = 6543217;
    Y = 987654321;
    Z = 43219876;
}

unsigned long long JKissRandEngine::Next()
{
    unsigned long long t = 698769069ULL * Z + C;

    X *= 69069;
    X += 12345;

    Y ^= Y << 13;
    Y ^= Y >> 17;
    Y ^= Y << 5;

    C = t >> 32;
    Z = t;

    return X + Y + Z;
}

void JLKiss64RandEngine::Reseed(unsigned long seed)
{
    X = 123456789123ULL ^ seed;
    Y = 987654321987ULL;
    Z1 = 43219876;
    Z2 = 6543217;
    C1 = 21987643;
    C2 = 1732654;
}

unsigned long long JLKiss64RandEngine::Next()
{
    X = 1490024343005336237ULL * X + 123456789;
    Y ^= Y << 21;
    Y ^= Y >> 17;
    Y ^= Y << 30;

    unsigned long long t = 4294584393ULL * Z1 + C1;
    C1 = t >> 32;
    Z1 = t;
    t = 4246477509ULL * Z2 + C2;
    C2 = t >> 32;
    Z2 = t;
    return X + Y + Z1 + (static_cast<unsigned long long>(Z2) << 32);
}

void PCGRandEngine::Reseed(unsigned long seed)
{
    state = seed;
    inc = seed;
}

unsigned long long PCGRandEngine::Next()
{
    unsigned long long oldstate = state;
    state = oldstate * 6364136223846793005ULL + (inc|1);
    unsigned int xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    unsigned int rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
