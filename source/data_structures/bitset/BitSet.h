#pragma once

#include <cstdint>
#include <iostream>


class BitSet {
private:
    uint8_t* data = nullptr;
    int32_t maxTolerance = -1;
    size_t countOfBuckets = 0;
    bool neutralNumber = false;

    void resize(size_t newCountOfBuckets);
    void copyFrom(const BitSet& other);
    void moveTo(BitSet&& other) noexcept;
    void free();

    size_t getBucketIndex(int32_t number) const;
    size_t getIndexInCurrentBucket(int32_t number) const;

    void setCountOfBuckets(size_t countOfBuckets);
    void setMaxTolerance(int32_t maxTolerance);
    void setNeutralNumber(bool pred);
    bool isOverTolerance(int32_t number) const;

public:
    BitSet();
    BitSet(size_t maxNumber);
    BitSet(const BitSet& other);
    BitSet(BitSet&& other) noexcept;
    BitSet& operator = (const BitSet& other);
    BitSet& operator = (BitSet&& other) noexcept;
    ~BitSet();

    void addNumber(int32_t number);
    void removeNumber(int32_t number);
    bool hasNumber(int32_t number) const;
    size_t getCountOfBuckets() const;
    int32_t getTolerance() const;
    BitSet getAddition() const;

    BitSet& operator &= (const BitSet& other);
    BitSet& operator |= (const BitSet& other);
    BitSet& operator %= (const BitSet& other);

    friend std::ostream& operator << (std::ostream& os, const BitSet& bitSet);
    friend std::istream& operator >> (std::istream& is, BitSet& bitSet);
};

BitSet operator & (const BitSet& bitSet1, const BitSet& bitSet2);
BitSet operator | (const BitSet& bitSet1, const BitSet& bitSet2);
