    

#ifndef _STATEITERATOR_H
#define _STATEITERATOR_H

#include <stdexcept>
#include <limits>
#include <stdint.h>

#include "StateSet.h"

namespace Ufta
{


    template <typename T>
    class StateIterator
    {
    public:
    
        StateIterator(StateSet<T> *set);
        StateIterator(StateIterator<T> &it);
        ~StateIterator();

        bool end();
        void next();
        T getCurrentState();

        StateIterator<T> &operator++();
        StateIterator<T> operator++(int dummy);
        T operator*();

    private:

        StateSet<T> *mStateSet;
        bool mIteratingDiscrete;
        T mDiscreteIndex;
        T mDiscreteOffset;
        T mDCCounter;
        T *mDCBits;
        T mDCBitCount;

    };




    template <typename T>
    StateIterator<T>::StateIterator(StateSet<T> *set)
    : mStateSet(set)
    , mIteratingDiscrete(false)
    , mDiscreteIndex(0)
    , mDiscreteOffset(0)
    , mDCCounter(0)
    , mDCBits(nullptr)
    , mDCBitCount(0) 
    {
        mIteratingDiscrete = !set->hasValidBitmask();
        if(!mIteratingDiscrete)
        {
            mDCBits = new T[sizeof(T)*8];

            // take apart bitmask into a list of don't-care-bits we can permutate
            T dcmask = mStateSet->getDCMask();
            T shift = 1;

            for(uint32_t i = 0; i < sizeof(T)*8; ++i)
            {
                if(dcmask & shift)
                {
                    mDCBits[mDCBitCount++] = shift;
                }

                shift <<= 1;
            }
        }
    }

    template <typename T>
    StateIterator<T>::StateIterator(StateIterator<T> &it)
    : mStateSet(it.mStateSet)
    , mIteratingDiscrete(it.mIteratingDiscrete)
    , mDiscreteIndex(it.mDiscreteIndex)
    , mDiscreteOffset(it.mDiscreteOffset)
    , mDCCounter(it.mDCCounter)
    , mDCBits(nullptr)
    , mDCBitCount(it.mDCBitCount) 
    {
        mDCBits = new T[sizeof(T)*8];
        
        for(uint32_t i = 0; i < mDCBitCount; ++i)
        {
            mDCBits[i] = it.mDCBits[i];
        }
    }

    template <typename T>
    StateIterator<T>::~StateIterator()
    {
        if(mDCBits != nullptr)
        {
            delete[] mDCBits;
        }
    }

    template <typename T>
    bool StateIterator<T>::end()
    {
        return mIteratingDiscrete && mDiscreteIndex >= mStateSet->getDiscreteStateCount();
    }

    template <typename T>
    void StateIterator<T>::next()
    {
        if(!mIteratingDiscrete)
        {
            if(mDCCounter < ((1 << mDCBitCount) - 1))
            {
                ++mDCCounter;

            }else
            {
                mIteratingDiscrete = true;
            }

        }else if(mDiscreteIndex < mStateSet->getDiscreteStateCount())
        {
            if(mDiscreteOffset < mStateSet->getDiscreteState(mDiscreteIndex).length - 1)
            {
                ++mDiscreteOffset;
            
            }else
            {
                mDiscreteOffset = 0;
                ++mDiscreteIndex;
            }
        }
    }

    template <typename T>
    T StateIterator<T>::getCurrentState()
    {
        if(end())
        {
            throw std::out_of_range("Tried to deref state iterator at end state");
        }

        T state;

        if(!mIteratingDiscrete)
        {
            state = mStateSet->getOnMask();

            for(T i = 0; i < mDCBitCount; ++i)
            {
                if(mDCCounter & (1 << i))
                {
                    state |= mDCBits[i];
                }
            }

        }else
        {
            state = mStateSet->getDiscreteState(mDiscreteIndex).value + mDiscreteOffset;
        }

        return state;
    }

    // pre-increment
    template <typename T>
    StateIterator<T> &StateIterator<T>::operator++()
    {
        next();

        return *this;
    }

    // post-increment
    template <typename T>
    StateIterator<T> StateIterator<T>::operator++(int dummy)
    {
        StateIterator<T> it(*this);

        next();

        return it;
    }

    template <typename T>
    T StateIterator<T>::operator*()
    {
        return getCurrentState();
    }

}

#endif



