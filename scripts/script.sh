data_t4_min=9
data_t4_max=15
data_t4_optimal=0

data_t3_min=10
data_t3_max=16
data_t3_optimal=0

data_t2_min=8
data_t2_max=14
data_t2_optimal=0

data_t1_min=8
data_t1_max=8
data_t1_optimal=0

max_error=read()

function run_opt_code() {
    vivado_hls -f script1.tcl
}

run_nrml_code

for ((i=$data_t4_max; i>=$data_t4_min; i--))
do
    sed -iE "s/<[0-9]+(.*data_t4)/<$i\1/g" opt/unsharp_mask.h
    run_opt_code
    error=`cat /home/animesh/opt/test_data/data.txt`
    if [[ "$error" -ge "$max_error" ]]; then
	data_t4_optimal=$i
	break;
    fi
done

for ((i=$data_t3_max; i>=$data_t3_min; i--))
do
    sed -iE "s/<[0-9]+(.*data_t3)/<$i\1/g" opt/unsharp_mask.h
    run_opt_code
    error=`cat /home/animesh/opt/test_data/data.txt`
    if [[ "$error" -ge "$max_error" ]]; then
	data_t3_optimal=$i
	break;
    fi
done

for ((i=$data_t2_max; i>=$data_t2_min; i--))
do
    sed -iE "s/<[0-9]+(.*data_t2)/<$i\1/g" opt/unsharp_mask.h
    run_opt_code
    error=`cat /home/animesh/opt/test_data/data.txt`
    if [[ "$error" -ge "$max_error" ]]; then
	data_t2_optimal=$i
	break;
    fi
done

for ((i=$data_t1_max; i>=$data_t1_min; i--))
do
    sed -iE "s/<[0-9]+(.*data_t1)/<$i\1/g" opt/unsharp_mask.h
    run_opt_code
    error=`cat /home/animesh/opt/test_data/data.txt`
    if [[ "$error" -ge "$max_error" ]]; then
	data_t1_optimal=$i
	break;
    fi
done

echo "t4 $data_t4_optimal" >> test_data/result.txt
echo "t3 $data_t3_optimal" >> test_data/result.txt
echo "t2 $data_t2_optimal" >> test_data/result.txt
echo "t1 $data_t1_optimal" >> test_data/result.txt
