#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H


#include "system.h"
#include "bitmap.h"

class PageProvider
{
  public:
    PageProvider();

    ~PageProvider();

    int GetEmptyPage(); // Bitmap find

    void ReleasePage(int numPage); // Bitmap clear

    int NumAvailPage(); // Search clear bits

  private:
    BitMap *pageavail; // NumPhysPages
};

#endif
