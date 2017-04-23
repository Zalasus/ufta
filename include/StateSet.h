
#ifndef _STATESET_H
#define _STATESET_H


namespace Ufta
{

    template <typename T> //TODO: enforce primitive types here
    class StateSet
    {
    public:

        StateSet();


        StateIterator getIterator();
   
        void addDiscreteState(T state);

        /**
         * Tries to find a bitmask that represents as many discrete states as possible.
         * The discrete states are then removed from the list of states and the bitmask
         * updated accordingly.
         */
        void reduce();

        /**
         * A state that has no valid bitmask and no discrete states is a null state.
         * It must not be used.
         */
        bool isNullState();

        
    private:

        T mDiscreteStateCount;
        T mDiscreteStates[];

        T mOnMask;  // if masks are ambiguous (same bits set in on as in off, mask is defined as not in use)
        T mOffMask;        
    };



    template <typename T>
    StateSet<T>::StateSet()
    : mDiscreteStateCount(0)
    , mDiscreteStates(nullptr)
    , mOnMask(0)
    , mOffMask(0)
    {

    }

    


}


#endif 