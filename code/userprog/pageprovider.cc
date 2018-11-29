#include "pageprovider.h"

PageProvider::PageProvider(){
  pageavail = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider(){
  delete pageavail;
}

int PageProvider::GetEmptyPage(){
  return pageavail->Find();
}

void PageProvider::ReleasePage(int numPage){
  pageavail->Clear(numPage);
}

int PageProvider::NumAvailPage(){
  return pageavail->NumClear();
}
