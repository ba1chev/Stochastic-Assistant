#include <stdexcept>

#include "BitSet.h"
#include "../../Constants.h"

BitSet::BitSet() {
    size_t buckets = this->getBucketIndex(MAX_TOLERANCE_BITSET) + 1;
    this->setCountOfBuckets(buckets);
    this->data = new uint8_t[this->getCountOfBuckets()] {};
    this->setNeutralNumber(false);
    this->setMaxTolerance(MAX_TOLERANCE_BITSET);
}

BitSet::BitSet(BitSet&& other) noexcept {
    moveTo(std::move(other));
}

BitSet::BitSet(const BitSet& other) {
    copyFrom(other);
}

BitSet::BitSet(size_t maxNumber) {
    this->setCountOfBuckets(this->getBucketIndex(maxNumber) + 1);
    this->setMaxTolerance(maxNumber);
    this->setNeutralNumber(false);
    this->data = new uint8_t[this->getCountOfBuckets()] {};
}

BitSet& BitSet::operator = (const BitSet& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

BitSet& BitSet::operator = (BitSet&& other) noexcept {
    if (this != &other) {
        free();
        moveTo(std::move(other));
    }
    return *this;
}

BitSet::~BitSet() {
    free();
}

void BitSet::free() {
    delete[] this->data;
    this->data = nullptr;
    this->setMaxTolerance(-1);
    this->setNeutralNumber(false);
    this->setCountOfBuckets(0);
}

void BitSet::resize(size_t newCountOfBuckets) {
    if (newCountOfBuckets < this->getCountOfBuckets()) {
        throw std::logic_error("New count of buckets must be greater");
    }

    uint8_t* newData = new uint8_t[newCountOfBuckets] {};

    for (size_t i = 0; i < this->getCountOfBuckets(); i++) {
        newData[i] = this->data[i];
    }

    this->setCountOfBuckets(newCountOfBuckets);
    delete[] this->data;
    this->data = newData;
}

void BitSet::copyFrom(const BitSet& other) {
    this->data = new uint8_t[other.getCountOfBuckets()];
    this->setCountOfBuckets(other.getCountOfBuckets());
    this->setMaxTolerance(other.getTolerance());
    this->setNeutralNumber(other.neutralNumber);

    for (size_t i = 0; i < this->getCountOfBuckets(); i++) {
        this->data[i] = other.data[i];
    }
}

void BitSet::moveTo(BitSet&& other) noexcept {
    this->data = other.data;
    this->setCountOfBuckets(other.getCountOfBuckets());
    this->setMaxTolerance(other.getTolerance());
    this->setNeutralNumber(other.neutralNumber);

    other.data = nullptr;
    other.setCountOfBuckets(0);
    other.setNeutralNumber(false);
    other.setMaxTolerance(-1);
}

size_t BitSet::getBucketIndex(int32_t number) const {
    return number / COUNT_OF_ELEMENTS_IN_ONE_BUCKET;
}

size_t BitSet::getIndexInCurrentBucket(int32_t number) const {
    return number % COUNT_OF_ELEMENTS_IN_ONE_BUCKET;
}

void BitSet::setCountOfBuckets(size_t countOfBuckets) {
    this->countOfBuckets = countOfBuckets;
}

size_t BitSet::getCountOfBuckets() const {
    return this->countOfBuckets;
}

void BitSet::setMaxTolerance(int32_t maxTolerance) {
    this->maxTolerance = maxTolerance;
}

void BitSet::setNeutralNumber(bool pred) {
    this->neutralNumber = pred;
}

bool BitSet::isOverTolerance(int32_t number) const {
    return number > this->getTolerance();
}

void BitSet::addNumber(int32_t number) {
    if (number == -1) {
        this->setNeutralNumber(true);
        return;
    }
    if (number < 0) {
        throw std::logic_error("The number is over the tolerance or it is negative");
    }

    size_t bucketIndex = this->getBucketIndex(number);
    if (this->getTolerance() != -1 && number > this->getTolerance()) {
        this->resize(bucketIndex + 1);
        this->setMaxTolerance(number);
    } else if (bucketIndex >= this->getCountOfBuckets()) {
        this->resize(bucketIndex + 1);
    }

    if (this->hasNumber(number)) return;
    size_t indexInBucket = this->getIndexInCurrentBucket(number);
    this->data[bucketIndex] |= (1 << indexInBucket);
}

void BitSet::removeNumber(int32_t number) {
    if (!this->hasNumber(number)) return;
    if (number == -1) {
        this->setNeutralNumber(false);
        return;
    } 

    size_t indexInBucket = this->getIndexInCurrentBucket(number);
    size_t bucketIndex = this->getBucketIndex(number);
    this->data[bucketIndex] &= (~(1 << indexInBucket));
}

BitSet& BitSet::operator &= (const BitSet& other) {
    size_t commonBuckets = std::min(other.getCountOfBuckets(), this->getCountOfBuckets());
    int32_t tolerance = (this->getTolerance() == -1) ? other.getTolerance() :
                  (other.getTolerance() == -1 ? this->getTolerance() : std::min(this->getTolerance(), other.getTolerance()));

    BitSet result;
    result.free();
    result.setCountOfBuckets(commonBuckets);
    result.data = new uint8_t[commonBuckets] {};
    result.setMaxTolerance(tolerance);
    result.setNeutralNumber(this->neutralNumber && other.neutralNumber);

    size_t limit = commonBuckets * COUNT_OF_ELEMENTS_IN_ONE_BUCKET;
    if (tolerance != -1 && (size_t)(tolerance + 1) < limit) limit = (size_t)(tolerance + 1);

    for (size_t i = 0; i < limit; i++) {
        if (this->hasNumber((int32_t)i) && other.hasNumber((int32_t)i)) {
            result.addNumber((int32_t)i);
        }
    }

    *this = result;
    return *this;
}

BitSet& BitSet::operator |= (const BitSet& other) {
    size_t targetBuckets = std::max(other.getCountOfBuckets(), this->getCountOfBuckets());
    int32_t tol = (this->getTolerance() == -1 || other.getTolerance() == -1) ? -1 : std::max(this->getTolerance(), other.getTolerance());

    BitSet result;
    result.free();
    result.setCountOfBuckets(targetBuckets);
    result.data = new uint8_t[targetBuckets] {};
    result.setMaxTolerance(tol);
    result.setNeutralNumber(this->neutralNumber || other.neutralNumber);

    size_t limit = targetBuckets * COUNT_OF_ELEMENTS_IN_ONE_BUCKET;
    if (tol != -1 && (size_t)(tol + 1) < limit) limit = (size_t)(tol + 1);

    for (size_t i = 0; i < limit; i++) {
        if (this->hasNumber((int32_t)i) || other.hasNumber((int32_t)i)) {
            result.addNumber((int32_t)i);
        }
    }

    *this = result;
    return *this;
}

BitSet& BitSet::operator %= (const BitSet& other) {
    size_t targetBuckets = this->getCountOfBuckets();
    int32_t tolerance = this->getTolerance();

    BitSet result;
    result.free();
    result.setCountOfBuckets(targetBuckets);
    result.data = new uint8_t[targetBuckets] {};
    result.setMaxTolerance(tolerance);
    result.setNeutralNumber(this->neutralNumber && !other.neutralNumber);

    size_t limit = targetBuckets * COUNT_OF_ELEMENTS_IN_ONE_BUCKET;
    if (tolerance != -1 && (size_t)(tolerance + 1) < limit) limit = (size_t)(tolerance + 1);

    for (size_t i = 0; i < limit; i++) {
        if (this->hasNumber((int32_t)i) && !other.hasNumber((int32_t)i)) {
            result.addNumber((int32_t)i);
        }
    }

    *this = result;
    return *this;
}

std::ostream& operator << (std::ostream& os, const BitSet& bitSet) {
    if (bitSet.hasNumber(-1)) {
        os << "{ " << "Neutral ";
    } else {
        os << "{ ";
    }

    for (size_t i = 0; i < bitSet.getCountOfBuckets() * 8; i++) {
        if (bitSet.hasNumber((int32_t)i)) {
            os << i << " ";
        }
    }

    os << "}";
    return os;
}

std::istream& operator >> (std::istream& is, BitSet& bitSet) {
    bool wantNeutralNumber = false;
    is >> wantNeutralNumber;
    size_t countOfNumbers = 0;
    int32_t currentNumber = 0;
    BitSet result;

    if (wantNeutralNumber) result.addNumber(-1);
    is >> countOfNumbers;

    for (size_t i = 0; i < countOfNumbers; i++) {
        is >> currentNumber;
        result.addNumber(currentNumber);
    }

    bitSet = result;
    return is;
}

int32_t BitSet::getTolerance() const {
    return this->maxTolerance;
}

bool BitSet::hasNumber(int32_t number) const {
    if (number == -1) {
        return this->neutralNumber;
    }
    if (number < 0) {
        throw std::logic_error("Negative number");
    }
    if (this->getBucketIndex(number) + 1 > this->getCountOfBuckets()) {
        return false;
    }

    size_t indexInBucket = this->getIndexInCurrentBucket(number);
    size_t bucketIndex = this->getBucketIndex(number);
    return (this->data[bucketIndex] & (1 << indexInBucket)) != 0;
}

BitSet operator & (const BitSet& bitSet1, const BitSet& bitSet2) {
    BitSet result = bitSet1;
    result &= bitSet2;
    return result;
}

BitSet operator | (const BitSet& bitSet1, const BitSet& bitSet2) {
    BitSet result = bitSet1;
    result |= bitSet2;
    return result;
}

BitSet BitSet::getAddition() const {
    if (this->getTolerance() == -1) {
        throw std::logic_error("Addition is undefined for unbounded BitSet");
    }

    size_t targetBuckets = this->getBucketIndex(this->getTolerance()) + 1;
    BitSet result;
    result.free();
    result.setCountOfBuckets(targetBuckets);
    result.data = new uint8_t[targetBuckets] {};
    result.setMaxTolerance(this->getTolerance());
    result.setNeutralNumber(false);

    for (size_t i = 0; i <= (size_t)this->getTolerance(); i++) {
        if (!this->hasNumber((int32_t)i)) {
            result.addNumber((int32_t)i);
        }
    }

    return result;
}
