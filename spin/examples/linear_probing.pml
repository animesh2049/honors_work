#define HIGH 40

bit arr[HIGH+1];
byte counter = 0;

inline hash_func() {
    byte number = 0; 
    /* 
       Random number is not taken because random number generation in promela
       will cause state space explosion. It will brute force for all the possible
       numbers in the range which is not desired
    */
    
    //do
    //:: number < HIGH -> number++
    //:: break
    //od
    
    do
    :: number < HIGH -> 
        if
        :: arr[number] == 0 -> atomic {
                if
                :: arr[number] == 0 -> arr[number] = 1; counter = counter + 1; break;
                :: else -> skip
                fi
            }
        :: else -> number++;
        fi
    :: else -> break
    od
}


proctype linear_prob() {
    byte i = 0;
    do
    :: i > 7 -> break
    :: else -> hash_func(); i++;
    od

}

init {
    atomic {
        run linear_prob();
        run linear_prob();
        run linear_prob();
//        run linear_prob();
//        run linear_prob();
    }
    _nr_pr == 1;
    byte temp_counter = 0;
    byte j = 0;
    do
    :: j > HIGH -> break
    :: else -> 
        if
        :: arr[j] == 1 -> temp_counter = temp_counter + 1;
        :: else -> skip
        fi
        j++;
    od
    assert(counter == temp_counter);
}
