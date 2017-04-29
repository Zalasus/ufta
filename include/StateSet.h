
#ifndef _STATESET_H
#define _STATESET_H

#include <stdexcept>
#include <string>
#include <limits>
#include <stdint.h>

namespace Ufta
{

    template <typename T> //TODO: enforce primitive types here
    class StateSet
    {
    public:

        struct DiscreteState { T value; T length; };

        StateSet();
        ~StateSet();
   
        void addDiscreteState(T state, T length);
        DiscreteState getDiscreteState(T index);
        T getDiscreteStateCount();

        void setBitmask(T onMask, T offMask);
        void setBitmask(const std::string &str);
        T getOnMask();
        T getOffMask();
        T getDCMask();

        /**
         * Tries to find a bitmask that represents as many discrete states as possible.
         * The discrete states are then removed from the list of states and the bitmask
         * updated accordingly.
         */
        void pack();

        /**
         * A state that has no valid bitmask and no discrete states is a null state.
         * It must not be used.
         */
        bool isNullState();

        bool hasValidBitmask();
        
    private:

        T mDiscreteStateCount;
        DiscreteState *mDiscreteStates;

        T mOnMask;  // if masks are ambiguous (same bits set in on as in off, mask is defined as not in use)
        T mOffMask;        
    };



    template <typename T>
    StateSet<T>::StateSet()
    : mDiscreteStateCount(0)
    , mDiscreteStates(nullptr)
    , mOnMask(1) // initially null-state
    , mOffMask(1)
    {

    }

    template <typename T>
    StateSet<T>::~StateSet()
    {
        if(mDiscreteStates != nullptr)
        {
            delete[] mDiscreteStates;
        }
    }

    template <typename T>
    void StateSet<T>::addDiscreteState(T state, T length)
    {
        //if(mDiscreteStateCount == std::numeric_limits<T>::max)
        //{
        //    throw std::out_of_range("Exceeded maximum discrete state count of state set");
        //}
        
        DiscreteState *allocArray = new DiscreteState[mDiscreteStateCount+1];
        
        // memcpy
        for(T i = 0; i < mDiscreteStateCount; ++i)
        {
            allocArray[i] = mDiscreteStates[i];
        }

        allocArray[mDiscreteStateCount].value = state;
        allocArray[mDiscreteStateCount].length = length;

        ++mDiscreteStateCount;

        delete[] mDiscreteStates;
        mDiscreteStates = allocArray;
    }

    template <typename T>
    typename StateSet<T>::DiscreteState StateSet<T>::getDiscreteState(T index)
    {
        if(index >= mDiscreteStateCount || index < 0)
        {
            throw std::out_of_range("Discrete state index out of bounds");
        }

        return mDiscreteStates[index];
    }

    template <typename T>
    T StateSet<T>::getDiscreteStateCount()
    {
        return mDiscreteStateCount;
    }

    template <typename T>
    void StateSet<T>::setBitmask(T onMask, T offMask)
    {
        mOnMask = onMask;
        mOffMask = offMask;
    }

    template <typename T>
    void StateSet<T>::setBitmask(const std::string &str)
    {
        T on = 0;
        T off = 0;

        for(uint32_t i = 0; i < str.length(); ++i)
        {
            on <<= 1;
            off <<= 1;

            if(str[i] == '0')
            {
                off |= 1;

            }else if(str[i] == '1')
            {
                on |= 1;
            }
        }

        mOnMask = on;
        mOffMask = off;
    }

    template <typename T>
    T StateSet<T>::getOnMask()
    {
        return mOnMask;
    }

    template <typename T>
    T StateSet<T>::getOffMask()
    {
        return mOffMask;
    }

    template <typename T>
    T StateSet<T>::getDCMask()
    {
        return ~(mOnMask | mOffMask);
    }

    template <typename T>
    void StateSet<T>::pack()
    {
        //TODO: think of fancy algorithm to do this
    }

    template <typename T>
    bool StateSet<T>::isNullState()
    {
        return !hasValidBitmask() && mDiscreteStateCount == 0;
    }

    template <typename T>
    bool StateSet<T>::hasValidBitmask()
    {
        // bitmask is valid if no bits are 1 in both on and off mask

        return !(mOnMask & mOffMask);
    }

}


#endif 