cat /sys/android_camera/sensor | \
while read line; do
    if [[ $line == *"imx298"* ]]; then
        setprop sys.product.rear_camera IMX298
    else
        if [[ $line == *"s5k2l7"* ]]; then
            setprop sys.product.rear_camera S5K2L7
        fi
    fi
done
