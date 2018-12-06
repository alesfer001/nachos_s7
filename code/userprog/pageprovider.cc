#include "pageprovider.h"

PageProvider::PageProvider(){
  pageavail = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider(){
  delete pageavail;
}

int PageProvider::GetEmptyPage(){
  int my_page = pageavail->Find();
  if(my_page != -1){
    memset(machine->mainMemory + (my_page * PageSize), 0, PageSize);
  }
  return my_page;
}

void PageProvider::ReleasePage(int numPage){
  pageavail->Clear(numPage);
}

int PageProvider::NumAvailPage(){
  return pageavail->NumClear();
}
