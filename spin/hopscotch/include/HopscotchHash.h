#ifndef HOPSCOTCHHASH_H
#define HOPSCOTCHHASH_H
#include <atomic>
#include <climits>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <mutex>
#include <bitset>
#include <sys/time.h>
#include "SpookyHash.h"
#include "fnv.h"
#include "emmintrin.h"

// 10 20 30 40 50 60

//10% -> 83886080
//20% -> 41943040
//40% -> 20971520
//60% -> 13981013
#define TotalBuckets   41943040 //13981013//10485759(10) //5991862(70) //6990506 (60%)//8388608(50%)//13981013 (30%)//20971520 (20%) //41943040 (10%)//          //2621440//1597967//5242879//1597967//////524287989//////2097151//3145727////109999//359999//13999
#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

class HopscotchHash
{
    public:
        std::atomic<unsigned long> TimeCounter;
        HopscotchHash();
        virtual ~HopscotchHash();
        unsigned int MurmurHash2A (const void * key, int len, unsigned int tabsize)
        {
            const unsigned int m = 0x5bd1e995;
            const int r = 24;
            unsigned int l = len;

            const unsigned char * data = (const unsigned char *)key;

            unsigned int h = seed;

            while(len >= 4)
            {
                unsigned int k = *(unsigned int*)data;

                mmix(h,k);

                data += 4;
                len -= 4;
            }

            unsigned int t = 0;

            switch(len)
            {
            case 3: t ^= data[2] << 16; break;
            case 2: t ^= data[1] << 8; break;
            case 1: t ^= data[0]; break;
            };

            mmix(h,t);
            mmix(h,l);

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h % tabsize;
        }


        unsigned int CreateBucketEntry(unsigned int key, unsigned long *NewEntry, short int threadID)
        {
            unsigned int InitialSlot =  MurmurHash2A((const void*)&key, 4, TotalBuckets);
            unsigned long Entry = 0;
            short int header=0;
            short int STATE = 3;
            STATE<<=14;
            header|=STATE;
            header|=threadID;

            //std::cout<< " HEADER  "<<std::bitset<16>(header)<<"\n";

            //std::cout << " ENTRY 0 " << std::bitset<32>(key) <<"  "<<key <<"\n";
            Entry|=key;
            //std::cout << " ENTRY 1 " << std::bitset<64>(Entry) << "\n";
            //Entry<<=31;
            //std::cout << " ENTRY 2 " << std::bitset<64>(Entry) << "\n";
            //Entry|=(1L<<63);
            Entry|=(0L|header)<<48;
            //std::cout << " ENTRY 3 " << std::bitset<64>(Entry) << "\n";
            //std::cout << " ENTRY 3 " << std::bitset<64>(Entry) << "\n";
            //std::cout << " SLOT " << std::bitset<32>(InitialSlot) << "  " <<InitialSlot <<"\n";


            //Entry|=((InitialSlot<<2)|3);
            //std::cout << " ENTRY 4 " << std::bitset<64>(Entry) << "\n";

            //unsigned int k = getKey(Entry);
            //std::cout << " key after extraction " << std::bitset<32>(k) <<"  "<< k<<"\n";

            //short int b = getHeader(Entry);
            //std::cout << " header after extraction " << std::bitset<16>(b) <<"  "<< b <<"\n";
            //unsigned long test = 1;
            //test<<=62;
            //std::cout << std::bitset<64>(test);
            *NewEntry = Entry;
            return InitialSlot;

        }
        unsigned int getKey(unsigned long Entry)
        {
                 long Mask = 0xFFFFFFFFFFFFL;
                 return (unsigned int)(Entry & Mask);
        }

        short int getHeader(unsigned long Entry)
        {

                return (short int)((Entry >> 48) & 0xFFFF);
        }

        unsigned int getBucket(unsigned int index)
        {
            unsigned int StoredKey = getKey(Buckets[index].load());
            return MurmurHash2A((const void*)&StoredKey, 4, TotalBuckets);
        }

        short int getThread(unsigned long Entry)
        {
            short int Header = getHeader(Entry);
            return (short int)(Header<<2)>>2;
        }

        void clearThread(unsigned long *Entry)
        {
                short int Header = getHeader(*Entry);
                *Entry=(*Entry<<16)>>16;
                 Header&=0xC000;
                *Entry|=(0L|Header)<<48;
        }

        unsigned int checkIntegrity(int num)
        {
            int counter=0;
            for(int i=0;i<TotalBuckets ; i++)
            if(Buckets[i].load()!=0)
            {
            //std::cout << " slot " << i << " key " << this->getKey(Buckets[i]) <<"\n";
            counter++;
            }

            //std::cout <<  " elements " << counter << " Difference ---> " << (num-counter)<<"\n ";
            std::cout <<  " elements " << counter <<"\n";
            return counter;
        }
        void containsS(unsigned int key)
        {

            unsigned int InitialSlot =  MurmurHash2A((const void*)&key, 4, TotalBuckets);


            unsigned int y, index;
            short int Header;
            unsigned long _SlotEntry=0;
            unsigned int t=0;

            for(unsigned int p=0;p<=H ; p++)
            {
                    index = InitialSlot + p;
                    _SlotEntry = Buckets[index].load();
                    /*Header = getHeader(_SlotEntry);
                    Header>>=14;
                    if(Header==0|| Header==1)
                    {*/
                        y = getKey(_SlotEntry);
                        if(key==y)
                        t++;
                    //}
            }

            if(t>1)
            std::cout<<"\n "<< key <<" has more count " <<t<<"\n";
       }
        void print_table()
        {
                for(int i=0;i<TotalBuckets ; i++)
                if(Buckets[i].load()!=0)
                {
                        std::cout<< this->getKey(Buckets[i].load()) << "\n";
                }
        }

        void Vectorize(int index, unsigned int key)
        {

                const __m128i Entries = _mm_loadu_si128((__m128i *)&Buckets[index]);
                unsigned long keys[2]={0};
                keys[0]|=key;
                keys[1]|=key;

                keys[0]<<=31;
                keys[1]<<=31;

                unsigned long mask[2];
                mask[0] = 0xffffffff00000000L>>1;
                mask[1] = 0xffffffff00000000L>>1;

                const __m128i mask128 =  _mm_loadu_si128((__m128i *)mask);
                __m128i ResultEntry = _mm_and_si128 (Entries, mask128);

                __m128i KEY = _mm_loadu_si128((__m128i *)keys);




                //std:: cout << " key loaded " << K8[2] <<"\n";
        }
        bool add(unsigned int key, int);
        bool Remove(unsigned int key);
        unsigned int Relocate(unsigned int RelocateEntry, long Entry);
        bool contains(unsigned int key);

        bool Eliminate(unsigned int key, int threadId, int, int );
        protected:
        private:
          std::atomic<unsigned long> *Buckets;
          int ADD_RANGE = 256;//512;
          unsigned int H = 32;//64;
          int KEYINFO = 28;
          unsigned int seed;
          SpookyHash spooky;
          std::mutex mutex;

};

#endif // HOPSCOTCHHASH_H
