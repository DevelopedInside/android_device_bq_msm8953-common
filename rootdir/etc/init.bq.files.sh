if [ -e /sys/bus/i2c/drivers/es9118-codec/6-0048 ]; then
        setprop sys.product.hifi hifi
else
        setprop sys.product.hifi no_hifi
fi

MemTotalStr=`cat /proc/meminfo | grep MemTotal`
MemTotal=${MemTotalStr:16:8}
((IsPremium=MemTotal>3145728?1:0))
if [ $IsPremium -eq 1 ]; then
    setprop sys.product.model_variant premium
else
    setprop sys.product.model_variant essential
fi

file="/cache/bq_reset"
if [ -f "$file" ]
then
	setprop persist.bq.reset 1
else
	setprop persist.bq.reset 0
fi
