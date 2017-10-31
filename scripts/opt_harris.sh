data_t6_min=29
data_t6_optimal=0

data_t5_min=17
data_t5_optimal=0

data_t4_min=29
data_t4_optimal=0

data_t3_min=17
data_t3_optimal=0

data_t2_min=14
data_t2_optimal=0

data_t1_min=8
data_t1_optimal=0

opt_fractional_bit=4

read required_accuracy
image_path=/home/animesh/opt_haris/images


function run_opt_code() {
    echo "calling vivado for optimal"
    vivado_hls -f script1.tcl
}

function run_nrml_code() {
    image_name=$1
    echo "calling vivado for nrml"
    cd ../HCD_ground
    sed -E -i "s|orig_img_path \".[^\"]*\"|orig_img_path \"$image_path/$image_name\"|g" const.h
    sed -E -i "s|ground_img_path \".[^\"]*\"|ground_img_path \"$PWD/$image_name\"|g" const.h
    vivado_hls -f script1.tcl
    cd ../HCD
}

function bin_search() {
    low=0
    high=32
    echo "$PWD"
    while [[ $low -lt $high ]]; do
        mid=$((($low + $high)/2))
        sed -i -E "s/<[0-9]+(.*data_t6)/<$(($mid+$data_t6_min))\1/g" harris.h
        sed -i -E "s/<[0-9]+(.*data_t5)/<$(($mid+$data_t5_min))\1/g" harris.h
        sed -i -E "s/<[0-9]+(.*data_t4)/<$(($mid+$data_t4_min))\1/g" harris.h
        sed -i -E "s/<[0-9]+(.*data_t3)/<$(($mid+$data_t3_min))\1/g" harris.h
        sed -i -E "s/<[0-9]+(.*data_t2)/<$(($mid+$data_t2_min))\1/g" harris.h
        sed -i -E "s/<[0-9]+(.*data_t1)/<$(($mid+$data_t1_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        echo "$mid $accuracy"
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l ) )) ; then
            opt_fractional_bit=$mid
            high=$mid
        else
            low=$(($mid+1))
        fi
    done
    echo "-------------------"
    echo "$accuracy $opt_fractional_bit"
    read temp
    echo "-------------------"
}

function set_bits() {
    sed -i -E "s/<[0-9]+(.*data_t6)/<$(($data_t6_min+$1))\1/g" harris.h
    sed -i -E "s/<[0-9]+(.*data_t5)/<$(($data_t5_min+$1))\1/g" harris.h
    sed -i -E "s/<[0-9]+(.*data_t4)/<$(($data_t4_min+$1))\1/g" harris.h
    sed -i -E "s/<[0-9]+(.*data_t3)/<$(($data_t3_min+$1))\1/g" harris.h
    sed -i -E "s/<[0-9]+(.*data_t2)/<$(($data_t2_min+$1))\1/g" harris.h
    sed -i -E "s/<[0-9]+(.*data_t1)/<$(($data_t1_min+$1))\1/g" harris.h

    data_t1_optimal=$1
    data_t2_optimal=$1
    data_t3_optimal=$1
    data_t4_optimal=$1
    data_t5_optimal=$1
    data_t6_optimal=$1
}

for image in `ls $image_path`; do
    run_nrml_code $image

    sed -E -i "s|orig_img_path \".[^\"]*\"|orig_img_path \"$image_path/$image\"|g" const.h
    ground_img_path="/home/animesh/opt_haris/Harris_corner/HCD_ground/$image"
    current_img_path="$PWD/$image"
    echo "$ground_img_path $current_img_path"
    sed -E -i "s|ground_img_path \".[^\"]*\"|ground_img_path \"$ground_img_path\"|g" const.h
    sed -E -i "s|current_img_path \".[^\"]*\"|current_img_path \"$current_img_path\"|g" const.h
    echo "$PWD"
    bin_search
    set_bits $opt_fractional_bit
    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t6)/<$(($i+$data_t6_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            data_t6_optimal=$i
        else
            echo "got saturation point for data_t6 $data_t6_optimal"
            break
        fi
        echo "for data_t6 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t6)/<$(($data_t6_optimal+$data_t6_min))\1/g" harris.h
    echo "-----------------"
    read temp
    echo "-----------------"
    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t5)/<$(($i+$data_t5_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            data_t5_optimal=$i
        else
            echo "got saturation point for data_t5 $data_t5_optimal"
            break
        fi
        echo "for data_t5 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t5)/<$(($data_t5_optimal+$data_t5_min))\1/g" harris.h
    echo "-----------------"
    read temp
    echo "-----------------"

    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t4)/<$(($i+$data_t4_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            echo "got saturation point for data_t4 $i"
            data_t4_optimal=$i
        else
            echo "got saturation point $data_t4_optimal"
            break
        fi
        echo "for data_t4 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t4)/<$(($data_t4_optimal+$data_t4_min))\1/g" harris.h
    echo "-----------------"
    read temp
    echo "-----------------"

    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t3)/<$(($i+$data_t3_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            data_t3_optimal=$i
        else
            echo "got saturation point for data_t3 $data_t3_optimal"
            break
        fi
        echo "for data_t3 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t3)/<$(($data_t3_optimal+$data_t3_min))\1/g" harris.h
    echo "-----------------"
    read temp
    echo "-----------------"

    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t2)/<$(($i+$data_t2_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            data_t2_optimal=$i
        else
            echo "got saturation point data_t2 $data_t2_optimal"
            break
        fi
        echo "for data_t2 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t2)/<$(($data_t2_optimal+$data_t2_min))\1/g" harris.h
    echo "-----------------"
    read temp
    echo "-----------------"

    for ((i=$opt_fractional_bit; i>=0; i--))
    do
        sed -i -E "s/<[0-9]+(.*data_t1)/<$(($i+$data_t1_min))\1/g" harris.h
        run_opt_code
        accuracy=`cat test_data/data.txt`
        if (( $(echo "$accuracy >= $required_accuracy" | bc -l) )); then
            data_t1_optimal=$i
        else
            echo "got saturation point data_t1 $data_t1_optimal"
            break
        fi
        echo "for data_t1 loop counter $i accuracy $accuracy"
    done
    sed -i -E "s/<[0-9]+(.*data_t1)/<$(($data_t1_optimal+$data_t1_min))\1/g" harris.h
    echo "$data_t6_optimal,$data_t5_optimal,$data_t4_optimal,$data_t3_optimal,$data_t2_optimal,$data_t1_optimal" >> test_data/results.txt
    echo "-------------------"
    echo "$ground_img_path $current_img_path"
    read temp
    rm $ground_img_path $current_img_path
    echo "-------------------"
done

