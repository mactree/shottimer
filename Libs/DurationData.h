#ifndef _DURATION_DATA_H_INCLUDED_
#define _DURATION_DATA_H_INCLUDED_

struct DurationData {
    DurationData(unsigned long seconds)
    {
        // secs >= 360000 --> 99:99 
        // secs >= 36000 --> hh:mm
        // secs >= 3600 --> _h:mm
        // secs >= 600 --> mm:ss
        // secs >= 60 --> _m:ss
        // secs < 10 --> ___s
        // secs < 60 --> __ss
        
        if (seconds >= 360000)
        {
            d0 = 9;
            d1 = 9;
            d2 = 9;
            d3 = 9;
            hasPoint = true;
        }
        else if (seconds >= 36000)
        {
            auto hours = seconds / 3600;
            auto mins = seconds % 3600 / 60;
            d0 = hours / 10;
            d1 = hours % 10;
            d2 = mins / 10;
            d3 = mins % 10;
            hasPoint = true;
        }
        else if (seconds >= 3600)
        {
            auto hours = seconds / 3600;
            auto mins = seconds % 3600 / 60;
            //d0 = hours / 10; --> hide leading zero
            d1 = hours % 10;
            d2 = mins / 10;
            d3 = mins % 10;
            hasPoint = true;
        }
        else if (seconds >= 600)
        {
            auto mins = seconds / 60;
            auto secs = seconds - mins * 60;
            d0 = mins / 10;
            d1 = mins % 10;
            d2 = secs / 10;
            d3 = secs % 10;
            hasPoint = true;
        }
        else if (seconds >= 60)
        {
            auto mins = seconds / 60;
            auto secs = seconds - mins * 60;
            //d0 = mins / 10; --> hide leading zero
            d1 = mins % 10;
            d2 = secs / 10;
            d3 = secs % 10;
            hasPoint = true;
        }
        else if (seconds < 10)
        {
            d3 = seconds;
        }
        else
        {
            d2 = seconds / 10;
            d3 = seconds % 10;
        }
    }

    int8_t d0 { 0x7f };
    int8_t d1 { 0x7f };
    int8_t d2 { 0x7f };
    int8_t d3 { 0x7f };
    bool hasPoint { false };
};
      
#endif // _DURATION_DATA_H_INCLUDED_
