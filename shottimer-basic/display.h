#ifndef _DISPLAY_H_INCLUDED_
#define _DISPLAY_H_INCLUDED_

#include <TM1637.h>
#include <../../shottimer/Libs/DurationData.h>

struct Display {
    Display(const int& clk, const int& dio) : _d(clk, dio) {
      _current[0] = _current[1] = _current[2] = _current[3] = 0x7f;
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
        for (auto& d : data) {
          if (d == 0) d = 0x7f;
          else break;
        }
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

    // displays "ST:00" up to "ST:99"
    void DrawShotTime(long seconds)
    {
      if (seconds > 99) seconds = 99;
      if (_current[0] != Display::Chars::S)
      {      
        _current[0] = Display::Chars::S;
        DrawRaw(0, _current[0] | 0x80);
      }
      if (_current[1] != Display::Chars::T)
      {      
        _current[1] = Display::Chars::T;
        DrawRaw(1, _current[1] | 0x80);
      }
      if (_current[2] != seconds / 10)
      {      
        _current[2] = seconds / 10;
        DrawRaw(2, _d.coding(_current[2]) | 0x80);
      }
      if (_current[3] != seconds % 10)
      {      
        _current[3] = seconds % 10;
        DrawRaw(3, _d.coding(_current[3]) | 0x80);
      }
    }
     
    void DrawOnTime(long totalSecs) {
      auto d = DurationData(totalSecs);
      _d.point(d.hasPoint);

      if (d.d0 != _current[0])
      {
        _current[0] = d.d0;
        _d.display(0, _current[0]);
      }
      if (d.d1 != _current[1])
      {
        _current[1] = d.d1;
        _d.display(1, _current[1]);
      }
      if (d.d2 != _current[2])
      {
        _current[2] = d.d2;
        _d.display(2, _current[2]);
      }
      if (d.d3 != _current[3])
      {
        _current[3] = d.d3;
        _d.display(3, _current[3]);
      }
    }

    enum Chars : int8_t {
      S = 1 + 32 + 64 + 4 + 8,
      T = 32 + 16 + 64 + 8,
      O = 1 + 2 + 4 + 8 + 16 + 32,
      N = 1 + 2 + 4 + 16 + 32
    };
    
    void DrawRaw(uint8_t BitAddr, int8_t data)
    {
      /*
       0x01 --> top
       0x02 --> top right
       0x04 --> bottom right
       0x08 --> bottom
       0x10 --> bottom left
       0x20 --> top left
       0x40 --> middle
       0x80 --> (point)
       */
      _d.start();
      _d.writeByte(ADDR_FIXED);
      _d.stop();
      _d.start();
      _d.writeByte(BitAddr|0xc0);
      _d.writeByte(data);
      _d.stop();
      _d.start();
      _d.writeByte(_d.Cmd_DispCtrl);
      _d.stop();
    }
    
  private:
    TM1637 _d;
    int8_t _current[4];
};



#endif // _DISPLAY_H_INCLUDED_
