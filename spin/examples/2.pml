mtype = {red, yellow, green};
mtype light = green;

active proctype P() {
    do
    :: if
       :: light == red -> light = green
       :: light == yellow -> light = red
       :: light == green -> light = yellow
       fi;
       printf("The ligth is now %e\n", light);
    od
}
