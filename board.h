#include <assert.h>

#include "direction.h"
#include "display.h"

template <int x0, int x1, int y0, int y1, int outvalue> class BitTable 
{
    static const int width  = x1 - x0 + 1;
    static const int height = y1 - y0 + 1;
    static const int n = width * height; // number of bits in table
    static const int tabSize = (n + 7) >> 3; // size of table in bytes
    int8_t table[tabSize];
  public : 
    BitTable() {
      for (int i=0; i<tabSize; i++) 
        table[i]=0;
      // TODO memset( table, 0, n);
    };
    int get(int x, int y) {
      if ((x<x0) or (x>x1) or (y<y0) or (y>y1))
        return outvalue;
      else { 
        const int i = (y-y0) * width + (x-x0); 
        return ( table[i >> 3] >> (i&7) ) & 1 ;
      }
    };
    int set(int x, int y, int v) {
      assert ((x>=x0) and (x<=x1) and (y>=y0) and (y<=y1));
      const int i = (y-y0) * width + (x-x0); 
      const int mask = 1 << (i&7);
      if (v) 
        table[i>>3] |= mask;
      else 
        table[i>>3] &= ~mask;
    };
    void print(char * title) {
      Serial.println(title);
      for(int y=y0; y<=y1; y++) {
        Serial.print("  ");
        for(int x=x0; x<=x1; x++) {
          Serial.print(get(x,y));
        }
        Serial.println("");
      }
    }
};

template <int w, int h> class Board {

public: // DEBUG
  BitTable<1,w-1,1,h-1,1> _point;
  //BitTable<1,w-2,1,h-1,1> _east;
  //BitTable<1,w-1,1,h-2,1> _south;
  BitTable<0,w,0,h,1> _east;
  BitTable<0,w,0,h,1> _south;
  
  Display * _display;
  
  bool get_point( int x, int y )             { return _point.get(x,y); }
  void set_point( int x, int y, bool value ) { _point.set(x,y,value); }

  bool get_east  ( int x, int y ) { return _east.get(x,y);   }
  bool get_west  ( int x, int y ) { return _east.get(x-1,y); }
  bool get_south ( int x, int y ) { return _south.get(x,y);  }
  bool get_north ( int x, int y ) { return _south.get(x,y-1);}

  void set_east  ( int x, int y, bool value ) { _east.set(x,y,value);   }
  void set_west  ( int x, int y, bool value ) { _east.set(x-1,y,value); }
  void set_south ( int x, int y, bool value ) { _south.set(x,y,value);  }
  void set_north ( int x, int y, bool value ) { _south.set(x,y-1,value);}

public :
  Board( Display * display ) {
    _display = display;
  }

  bool CanSet( int x, int y ) {
    return get_point(x,y) == false; // true if point is free  
  }

  bool CanMove( int x, int y, Direction dir ) {
    move(x,y,dir); // compute point in direction <dir>
    bool c = CanSet(x,y);
    return c;  
  }
  
  void SetPoint( int x, int y, int16_t color ) {
    _display->DrawPoint(x,y,color);
    set_point(x,y,true); // set point to not free  
  }

  void SetSegment(int x0, int y0, Direction dir, int16_t color ) {
    
    int x1;
    int y1;
    
    move(x0,y0,dir,x1,y1);
    _display->DrawLine(x0,y0,x1,y1, color);
    
    // save state of segment from position to direction
    switch(dir) {
      case North :
        set_north(x0,y0,true);
        break;
      case South :
        set_south(x0,y0,true);
        break;
      case East :
        set_east(x0,y0,true);
        break;
      case West :
        set_west(x0,y0,true);
        break;
    }
    
    set_point(x1,y1,true); // set point to not free  
  }

  void ResetSegment(int x0, int y0, Direction dir ) {
    
    int x1;
    int y1;
    
    move(x0,y0,dir,x1,y1);
    _display->ClearLine(x1,y1,x0,y0);
    
    // save state of segment from position to direction
    switch(dir) {
      case North :
        set_north(x0,y0,false);
        break;
      case South :
        set_south(x0,y0,false);
        break;
      case East :
        set_east(x0,y0,false);
        break;
      case West :
        set_west(x0,y0,false);
        break;
    }
    
    set_point(x1,y1,false);   
  }

  void ClearLine(int x, int y ) {
    
    Direction dir;
    
    while (true) {
      set_point(x,y,false); 
      if (get_east(x,y) == 1) 
        dir = East;
      else if (get_west(x,y) == 1) 
        dir = West;
      else if (get_north(x,y) == 1) 
        dir = North;
      else if (get_south(x,y) == 1) 
        dir = South;
      else {
        _display->ClearPoint(x,y);
        return;
      }
      
      ResetSegment( x, y, dir );
      move(x,y,dir);
    }
  }
    void print() {
      _point.print( "Point" );
      _east.print("East");
      _south.print("South");
    }

};

