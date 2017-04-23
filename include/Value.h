

namespace Ufta
{


    class Value
    {
    public:

        enum OriginType
        {
            ORIGIN_MEMORY,
            ORIGIN_IMMEDIATE,
            ORIGIN_REGISTER
        };

        
    
        
    private:

        StateSet mBaseValue;
        OriginType mValueOrigin;
        Operation *mOpStack;

    };


}