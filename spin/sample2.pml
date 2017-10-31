#define NOPROC 4
#define INF 1000
typedef Pair {
	int key, tid;
}

Pair mypair[5];

proctype insert(int mytid; int slot;int mykey){
	bool retVal = true
	int temp = 1, varkey, vartid
	do
	:: temp > NOPROC -> break
    :: else ->    /* Putting everything in else because the latter statements should be executed only after the previous check */
        if
        :: ((mypair[temp].key == mykey) && (mypair[temp].tid < mytid)) -> retVal = false; temp = NOPROC + 1 ; break /* return false */
	    :: ((mypair[temp].tid > mytid) && (mypair[temp].key == mykey) && (mypair[temp].tid != INF)) -> 
	      vartid = mypair[temp].tid; varkey = mypair[temp].key;
	      d_step {
		   if
		   :: ((vartid == mypair[temp].tid) && (vartid != 0)) -> mypair[temp].tid = INF
		   :: ((mypair[temp].tid == 0) && (mypair[temp].key == mykey)) -> retVal = false     /*return false*/
           :: else -> skip
		   fi
	      }; temp = temp + 1
        :: else -> temp = temp + 1 // Confirm with sir
        fi
	od

	if
	:: retVal -> 
		 d_step {
	 	 if
	 	 :: (mypair[slot].tid != mytid) -> mypair[slot].key = 0; mypair[slot].tid = 0
	 	 :: (mypair[slot].tid == mytid) -> 
	 	     mypair[slot].tid = 0;
	 	     mypair[slot].key = mykey;
         :: else -> skip
	 	 fi
	 	 }	 
	:: else -> atomic {
	    	 mypair[slot].key = 0
		     mypair[slot].tid = 0
		    }
	fi
}


init {
	int x = 1, key = 10
	do
	:: x > NOPROC -> break
	:: else -> mypair[x].key = 10 ; mypair[x].tid = x;x = x + 1
	od
	
	int y = 1
	do
	:: y > NOPROC -> break
	:: else -> run insert(y, y, key); y = y + 1
	od
	
	(_nr_pr == 1)
	int z = 1, counter1 = 0, counter2 = 0
	do
	:: z > NOPROC -> break
	:: else -> 
        if
        :: (mypair[z].key == 10) -> counter1 = counter1 + 1
        :: (mypair[z].tid != 0) -> counter2 = counter2 + 1
        :: else -> skip
        fi;
        z = z + 1
	od
	
	assert((counter1 == 1) && (counter2 == 0))
}
