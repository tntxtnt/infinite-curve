#include "VertexDeque.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

VertexContiguousDeque::VertexContiguousDeque(sf::PrimitiveType type,
                                             size_t vertexCount)
: mPtr(vertexCount ? new sf::Vertex[vertexCount] : nullptr),
  mPrimitiveType(type),
  mSize(vertexCount)
{}

VertexContiguousDeque::~VertexContiguousDeque()
{
    delete[] mPtr;
}

size_t VertexContiguousDeque::getVertexCount() const
{
    return mSize;
}

sf::Vertex& VertexContiguousDeque::operator[](size_t index)
{
    return mPtr[index + mSpaceFront];
}

const sf::Vertex& VertexContiguousDeque::operator[](size_t index) const
{
    return mPtr[index + mSpaceFront];
}

void VertexContiguousDeque::centerize(size_t deltaSize)
{
    auto pRead = mPtr + mSpaceFront;
    size_t capacity = mSpaceFront + mSize + mSpaceBack;
    size_t newSpaceFront = (capacity - mSize - deltaSize) / 2;
    auto pWrite = mPtr + newSpaceFront;
    for (size_t i = 0; i < mSize; ++i) *pWrite++ = *pRead++;
    // Reset spaces
    mSpaceFront = newSpaceFront;
    mSpaceBack = capacity - mSize - mSpaceFront;
}

void VertexContiguousDeque::expand(size_t newCapacity, bool centerCopy)
{
    size_t capacity = mSpaceFront + mSize + mSpaceBack;
    if (newCapacity <= capacity) return centerize();

    auto newPtr = new sf::Vertex[newCapacity];
    // Copy vertices from current holder to new one
    size_t newSpaceFront = centerCopy ? (newCapacity - mSize) / 2 : 0;
    std::copy(mPtr + mSpaceFront, mPtr + mSpaceFront + mSize,
              newPtr + newSpaceFront);
    // Swap pointers and delete swapped pointer
    std::swap(mPtr, newPtr);
    delete[] newPtr;
    // Reset spaces
    mSpaceFront = newSpaceFront;
    mSpaceBack = newCapacity - mSize - mSpaceFront;
}

void VertexContiguousDeque::resize(size_t vertexCount)
{
    if (vertexCount > mSize)
    {
        size_t capacity = mSpaceFront + mSize + mSpaceBack;
        if (vertexCount > capacity)
        {
            expand(vertexCount, false);
            // Reset sizes
            mSize = vertexCount;
            mSpaceFront = mSpaceBack = 0; // sf::Vertex is trivial class
        }
        else
        {
            centerize(vertexCount - mSize);
            // Reset spaces
            mSize = vertexCount;
            mSpaceBack = capacity - mSize - mSpaceFront;
        }
    }
    else
    {
        // Simply decrease mSize since sf::Vertex is trivial
        mSize = vertexCount;
    }
}

void VertexContiguousDeque::append(const sf::Vertex& vertex)
{
    if (!mSpaceBack) expand(mSize * 3);
    mPtr[mSpaceFront + mSize] = vertex;
    mSpaceBack--;
    mSize++;
}

void VertexContiguousDeque::prepend(const sf::Vertex& vertex)
{
    if (!mSpaceFront) expand(mSize * 3);
    mPtr[mSpaceFront - 1] = vertex;
    mSpaceFront--;
    mSize++;
}

void VertexContiguousDeque::setPrimitiveType(sf::PrimitiveType type)
{
    mPrimitiveType = type;
}

sf::PrimitiveType VertexContiguousDeque::getPrimitiveType() const
{
    return mPrimitiveType;
}

void VertexContiguousDeque::removeFirst()
{
    if (!mSize) return;
    mSpaceFront++;
    mSize--;
}

void VertexContiguousDeque::removeLast()
{
    if (!mSize) return;
    mSpaceBack++;
    mSize--;
}

void VertexContiguousDeque::draw(sf::RenderTarget& target,
                                 sf::RenderStates states) const
{
    if (mSize) target.draw(mPtr + mSpaceFront, mSize, mPrimitiveType, states);
}
