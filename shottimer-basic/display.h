#ifndef _DISPLAY_H_INCLUDED_
#define _DISPLAY_H_INCLUDED_

#include <TM1637.h>
#include <../../shottimer/Libs/DurationData.h>

struct Display {
    Display(const int& clk, const int& dio) : _d(clk, dio) {
    }

    void Init() {
      _d.init();
      _d.set(BRIGHT_TYPICAL);
    }

    enum class Brightness
    {
      Dark = BRIGHT_DARKEST,
      Typical = BRIGHT_TYPICAL,
      Bright = BRIGHTEST
    };

    void SetBrightness(Brightness b) {
      _d.set((int) b);
    }
    
    void Clear() {
      _d.clearDisplay();
    }

    void DrawNum(uint16_t n, bool showLeadingZero = false) {
      int8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
      data[0] = n / 1000;
      data[1] = (n - data[0] * 1000) / 100;
      data[2] = (n - data[0] * 1000 - data[1] * 100) / 10;
      data[3] = n % 10;

      if (!showLeadingZero) {
        removeLeadingZero(data);
      }
      
      _d.point(false);
      _d.display(data);
    }

    /*void DrawTime(uint8_t hours, uint8_t mins) {
      int8_t data[] = { 0, 0, 0, 0 };
      data[0] = hours / 10;
      data[1] = hours % 10;
      data[2] = mins / 10;
      data[3] = mins % 10;
      _d.point(true);
      _d.display(data);
    }

    void DrawTime(long totalSecs) {
      unsigned int totalMins = totalSecs / 60;
      unsigned int hours = totalMins / 60;
      unsigned int mins = totalMins - hours * 60;

      DrawTime(hours, mins);
    }*/
     
    void DrawDuration(long totalSecs) {
      auto d = DurationData(totalSecs);
      int8_t data[] = { d.d0, d.d1, d.d2, d.d3 };
      _d.point(d.hasPoint);
      _d.display(data);
    }
    
  private:
    
    template<typename T, size_t N>
    void removeLeadingZero(T (&data)[N]) {
      for (auto& d : data) {
        if (d == 0) d = 0x7f;
        else break;
      }
    }
    
    TM1637 _d;
};



#endif // _DISPLAY_H_INCLUDED_
