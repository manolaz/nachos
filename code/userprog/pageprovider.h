#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "utility.h"
#include "openfile.h"
#include "bitmap.h"

class PageProvider
{
private:
	BitMap *bitmapPage;

public:
	PageProvider();
	~PageProvider();

	int GetEmptyPage();
	void ReleasePage(int page);
	int NumAvailPage();

	int GetRandomEmptyPage(); // debuggage
};

#endif // PAGEPROVIDER_H

#endif // CHANGED