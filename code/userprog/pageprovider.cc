#ifdef CHANGED

#include <stdlib.h>

#include "pageprovider.h"
#include "machine.h"
#include "system.h"

PageProvider::PageProvider()
{
	bitmapPage = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider()
{
	delete bitmapPage;
}

int
PageProvider::GetEmptyPage()
{
	int emptyPage = bitmapPage->Find();
	ASSERT(emptyPage != -1);
	void *adresse = &(machine->mainMemory[emptyPage * PageSize]);
	memset(adresse, 0, PageSize);
	return emptyPage;
}

void
PageProvider::ReleasePage(int page)
{
	bitmapPage->Clear(page);
}

int
PageProvider::NumAvailPage()
{
	return bitmapPage->NumClear();
}


/* a utiliser que pour le debuggage
 * retourne un page aleatoire vide
*/
int
PageProvider::GetRandomEmptyPage()
{
	int randomPage;
	do
		randomPage = rand() % NumPhysPages;
	while(bitmapPage->Test(randomPage));
	bitmapPage->Mark(randomPage);
	void *adresse = &(machine->mainMemory[randomPage * PageSize]);
	memset(adresse, 0, PageSize);
	return randomPage;
}

#endif // CHANGED