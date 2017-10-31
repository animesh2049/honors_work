#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::memory_order_relaxed
#include <thread>
#include <algorithm>
#include <climits>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <fstream>
#include <map>
#include <mutex>
#include "HopscotchHash.h"
#define THREADS 8//32//8//16
#define TOTDATA 1024*1024*8
#define DATAPERTHREAD TOTDATA/THREADS
#define SINGLE 13107
#define RED 235929
#define dupc 100000
struct timeval start, stop;
using namespace std;
std::atomic<int> datacounter (0);
unsigned int *dataset;
int contains, inserts, deletes;
unsigned int duplicates[dupc];
std::mutex mu;
void insertKey(HopscotchHash *h, int tid)
{
unsigned int key = 0;
        for(int i=0;i<DATAPERTHREAD;i++)
        {

             key=tid*DATAPERTHREAD + i  +1;
             if(h->add(key, tid))
             dataset[datacounter.fetch_add(1)]=key;

        }
}

void checkKeys(HopscotchHash *h, int index)
{

    for(int i=0;i<DATAPERTHREAD;i++)
    {

            //h->containsS(dataset[index*DATAPERTHREAD+i]);
            if(!h->contains(dataset[index*DATAPERTHREAD+i]))
           std::cout <<" FAILURE " << dataset[index*DATAPERTHREAD+i]<<"\n ";


    }

}

void worker(HopscotchHash *h, int tid)
{
    int key=0;
    for(int i=0;i<contains;i++)
    {
        int index = rand() % datacounter.load();
        unsigned int key = dataset[index];
        h->contains(key);


    }

    for(int x=0;x<inserts;x++)
    {
        key = rand() % INT_MAX +1;

        h->add(key,tid);
    }



    for(int x=0;x<deletes;x++)
    {
        int index = tid*deletes + x;
        unsigned int key = dataset[index];
        //h->Remove(key);
        if(!h->Remove(key))
        {
            if(h->contains(key))
            cout<<" ERROR: Removal Failure for key " << key << "   " <<h->contains(key) << " by thread "<< tid << "\n" ;
            //exit(0);
        }
    }

}
void addDuplicates(HopscotchHash *h, int tid)
{
    for(int i=0;i<dupc;i++)
    h->add(duplicates[i], tid);
}

void checkDuplicates(HopscotchHash *h)
{

    for(int i=0;i<dupc;i++)
    {
            h->containsS(duplicates[i]);
            if(!h->contains(duplicates[i]))
            std::cout <<" FAILURE " << duplicates[i]<<"\n ";
    }

}
int main ()
{
  dataset = new unsigned int [TOTDATA];

  for(int i=0;i<TOTDATA ;i++)
  dataset[i]=0;
  HopscotchHash h;

  std::vector<std::thread> Threads;
  std::vector<std::thread> ContainThreads;


  for(int i=0;i<THREADS;i++)
  {
        Threads.push_back(std::thread(insertKey, &h, i));
  }
  gettimeofday(&start, NULL);
  for(int i=0;i<THREADS;i++)
  {
        Threads[i].join();
  }

  gettimeofday(&stop, NULL);
  printf("build time  %ld  MQPS \n",  (int)TOTDATA/((stop.tv_sec * 1000000 + stop.tv_usec)
	            				 - (start.tv_sec * 1000000 + start.tv_usec)));
  for(int i=0;i<THREADS;i++)
  {
        ContainThreads.push_back(std::thread(checkKeys, &h, i));
  }
  gettimeofday(&start, NULL);
  for(int i=0;i<THREADS;i++)
  {
        ContainThreads[i].join();
  }

  gettimeofday(&stop, NULL);
  long seconds = stop.tv_sec - start.tv_sec;
  long nsec = stop.tv_usec - start.tv_usec;
  cout<<"Time for full retreival "<<TOTDATA/(((seconds*1000000)+(nsec)))<<endl;


   cout<<"\n [90-5-5] (1) [80-10-10] (2) [60-30-30] (3) [30-30-30] (4) \n";
   int choice;
   cin>>choice;
   switch(choice)
   {
        case 1:
        {
            contains = (int)(TOTDATA*0.9)/THREADS;
            inserts =  (int)(TOTDATA*0.05)/THREADS;
            deletes =  (int)(TOTDATA*0.05)/THREADS;
            break;
        }
        case 2:
        {
            contains = (int)(TOTDATA*0.8)/THREADS;
            inserts =  (int)(TOTDATA*0.1)/THREADS;
            deletes =  (int)(TOTDATA*0.1)/THREADS;
            break;
        }
        case 3:
        {
            contains = (int)(TOTDATA*0.6)/THREADS;
            inserts =  (int)(TOTDATA*0.2)/THREADS;
            deletes =  (int)(TOTDATA*0.2)/THREADS;
            break;
        }
        case 4:
        {
            contains = (int)(TOTDATA*0.4)/THREADS;
            inserts =  (int)(TOTDATA*0.3)/THREADS;
            deletes =  (int)(TOTDATA*0.3)/THREADS;
            break;
        }



   }
            std::vector<std::thread> threads;
            for(int i=0;i<THREADS;i++)
            {
                threads.push_back(std::thread(worker, &h, i));
            }
            gettimeofday(&start, NULL);
            for(int i=0;i<THREADS;i++)
            threads[i].join();
            gettimeofday(&stop, NULL);
            seconds = stop.tv_sec - start.tv_sec;
            nsec = stop.tv_usec - start.tv_usec;
    if(choice==1)
            cout<<"Time for 90 % contains 5% add and 5 % delete "<<TOTDATA/(((seconds*1000000)+(nsec)))<<endl;
    else if(choice==2)
            cout<<"Time for 80 % contains 10% add and 10 % delete "<<TOTDATA/(((seconds*1000000)+(nsec)))<<endl;
    else if(choice==3)
            cout<<"Time for 60 % contains 20% add and 20% delete "<<TOTDATA/(((seconds*1000000)+(nsec)))<<endl;
    else
            cout<<"Time for 40 % contains 30% add and 30% delete "<<TOTDATA/(((seconds*1000000)+(nsec)))<<endl;

            /*unsigned int elements = h.checkIntegrity(0);

            for(int i=0;i<dupc;i++)
            {
                unsigned int key=0;
                do
                {
                        key = rand()% INT_MAX + 1;
                }while(h.contains(key));
                duplicates[i]=key;
            }

            std::vector<std::thread> threads0;
            for(int i=0;i<THREADS;i++)
            {
                threads0.push_back(std::thread(addDuplicates, &h, i+1));
            }

            for(int i=0;i<THREADS;i++)
            threads0[i].join();

            unsigned int x = h.checkIntegrity(0);*/

            /*if((elements+dupc)==x)
            std::cout<<" DUPLICATE TEST SUCCESS " <<"\n";
            else
            std::cout<<" DUPLICATE TEST FAILURE "<< ((elements+dupc) - x) <<"\n";*/
            //std::cout<<"\n\n######################### "<<  ((elements+dupc)-x) <<" #################################################################\n";
            //checkDuplicates(&h);
    return 0;
}
