#include "HopscotchHash.h"

HopscotchHash::HopscotchHash()
{
    srand(time(NULL));

    seed = rand() % LONG_MAX +1;
    Buckets = (std::atomic<unsigned long> *)malloc(sizeof(std::atomic<unsigned long>) * TotalBuckets + ADD_RANGE);
    for(int i=0;i<TotalBuckets + H; i++)
    Buckets[i].store(0);
    TimeCounter.store(0);
}

HopscotchHash::~HopscotchHash()
{

}

bool HopscotchHash::add(unsigned int key, int threadID)
{

        unsigned long  Entry;

        //bool done = false;
        unsigned long tag=0;
        unsigned int SlotIndex = CreateBucketEntry(key, &Entry, threadID);
        unsigned int BaseBucket = SlotIndex;
        int trails=0;

        if(contains(key))
        {
            //std::cout<<" duplicate "<<"\n";
            return false;
        }

        do
        {
            while(trails++ < ADD_RANGE && Buckets[SlotIndex].load()!=0)
            {
                //SlotIndex=(SlotIndex+1)%TotalBuckets;
                SlotIndex++;
            }
        }while(!Buckets[SlotIndex].compare_exchange_strong(tag,Entry) && trails < ADD_RANGE);

        if(trails >= ADD_RANGE)
        {
            std::cout<< " insertion failure "  << trails;
            exit(0);
        }

        if((SlotIndex - BaseBucket)> H)
        {
                do
                {
                unsigned int prevSlot = SlotIndex;
                SlotIndex = this->Relocate(SlotIndex, Entry);
                    if(prevSlot == SlotIndex)
                    {
                        std::cout<< " ERROR:: Caught in infinite loop ";
                        exit(0);
                    }

                }while((SlotIndex - BaseBucket) > H);


        }
        unsigned long ExpEntry, FinalEntry;
        short int _SlotThread;
        if(!Eliminate(key, threadID, SlotIndex, BaseBucket))
        {
            do
            {
                ExpEntry = Buckets[SlotIndex].load();
                _SlotThread = getThread(ExpEntry);
                if(_SlotThread!=threadID)
                    goto EXIT;
                FinalEntry = (ExpEntry<<2)>>2;
                clearThread(&FinalEntry);
            }while(!Buckets[SlotIndex].compare_exchange_strong(ExpEntry, FinalEntry));
            return true;
        }

        EXIT:do
        {
            ExpEntry = Buckets[SlotIndex].load();

        }while(!Buckets[SlotIndex].compare_exchange_strong(ExpEntry,0));
        return false;
}

bool HopscotchHash::Eliminate(unsigned int key, int threadId, int mySLot, int initialSlot)
{

    unsigned int _SlotKey;
    short int _SlotThread;
    unsigned long _SlotEntry,FinalEntry;
    for(int p=0;p<H;p++)
    {
        _SlotEntry= Buckets[initialSlot + p].load();
        _SlotKey = getKey(_SlotEntry);
        _SlotThread = getThread(_SlotEntry);

        if((initialSlot + p)!=mySLot)
        {

            if(_SlotThread < threadId && _SlotKey == key)
            {
                //std::cout << " Slot Key " << _SlotKey << " key " << key <<" Threads "<< _SlotThread << "  "<<threadId<<"\n";
                //getchar();
                return true;
            }

            if(_SlotThread > threadId && _SlotKey == key)
            {
                FinalEntry = _SlotEntry;
                clearThread(&FinalEntry);
                if(!Buckets[initialSlot + p].compare_exchange_strong(_SlotEntry, FinalEntry))
                {
                        _SlotKey = getKey(Buckets[initialSlot + p].load());
                        if(_SlotKey==key)
                        {
                            return true;
                        }
                }
            }
        }

    }
    return false;
}

unsigned int HopscotchHash::Relocate(unsigned int Loc, long NewEntry)
{
unsigned int Replace = Loc - H;
unsigned long Exp, Fin, Dest;
unsigned int Ind=Replace;
do{
 unsigned int BaseLoc = getBucket(Ind);
 unsigned int HopDist = (Loc - BaseLoc);
  while(HopDist >= H && Replace < Loc){
  Replace++;
  BaseLoc=getBucket(Replace);
  HopDist = (Loc - BaseLoc);
  }
if(HopDist > H || Replace >= Loc){ std::cout<< "\n\nRESIZE()\n\n"; exit(0); }
Exp = Buckets[Replace].load();
Exp = ((Exp << 2) >> 2);
Fin = ((Exp << 2) >> 2)| (1L<<62);
Ind = Replace+1;
}while(!Buckets[Replace].compare_exchange_strong(Exp, Fin));

do{
 Dest = Buckets[Loc].load();
 Dest = ((Dest << 2) >>2)| (3L<<62);
 Fin = Buckets[Replace].load();
 Fin = ((Fin<<2)>>2);
}while(!Buckets[Loc].compare_exchange_strong(Dest, Fin));

do{
 Exp = Buckets[Replace].load();
 Exp = ((Exp <<2 )>>2) | (1L<<62);
}while(!Buckets[Replace].compare_exchange_strong(Exp, NewEntry));

return Replace;
}

bool HopscotchHash::contains(unsigned int key)
{

       unsigned int InitialSlot =  MurmurHash2A((const void*)&key, 4, TotalBuckets);


       unsigned int y, index;
       short int Header;
       unsigned long _SlotEntry=0;
       //_SlotEntry|=key;

       for(unsigned int p=0;p<=H ; p++)
       {
            index = InitialSlot + p;
           _SlotEntry = Buckets[index].load();
           //if(index >= TotalBuckets)
             //  break;
           //else
           //{

            Header = getHeader(_SlotEntry);
            Header>>=14;
            if(Header==0|| Header==1)
            {
                    y = getKey(_SlotEntry);
                    if(key==y)
                    return true;
            }
           //}
           //;
       }
       return false;
}

bool HopscotchHash::Remove(unsigned int key)
{
        unsigned int InitialSlot =  MurmurHash2A((const void*)&key, 4, TotalBuckets);
        unsigned long _SlotEntry=0;

        bool rem = false;
        unsigned long random;
        //mutex.lock();
        //std::cout<<" Removing  " << key << "\n";
       //for(int p=0;p<HOPSCOTCH;p++)
        //{

        //}


        for(int p=0;p<=H;p++)
        {

            _SlotEntry=key;
            //_SlotEntry = (Buckets[InitialSlot + p].load()<<2)>>2;
            rem = Buckets[InitialSlot+p].compare_exchange_strong(_SlotEntry,0);
            /*if(Buckets[InitialSlot+p].compare_exchange_strong(_SlotEntry,0))
            {

                //std::cout<<" + " <<"\n";
                //std::cout<< Buckets[InitialSlot+p].load() << "   "<<_SlotEntry << "\n";
                rem = true;
            }*/


            _SlotEntry|=(1L<<62);

            rem = Buckets[InitialSlot+p].compare_exchange_strong(_SlotEntry,0);
            /*if(Buckets[InitialSlot+p].compare_exchange_strong(_SlotEntry,0))
            {
                std::cout<<" - " <<"\n";
                rem = true;
            }*/

            _SlotEntry = Buckets[InitialSlot + p].load();
            _SlotEntry=((_SlotEntry<<2)>>2)|(3L << 62);
            //random = rand() % LONG_MAX +1;
            random = INT_MAX;

            random=((random<<2)>>2)|(3L << 62);

            Buckets[InitialSlot + p].compare_exchange_strong(_SlotEntry, random);
            //if(Buckets[InitialSlot + p].compare_exchange_strong(_SlotEntry, random))
            //std::cout<<" * " <<"\n";
        }
        //mutex.unlock();
        return rem;

}


