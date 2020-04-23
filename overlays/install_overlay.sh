#Mount config filesystem
mount -t configfs none /sys/kernel/config

#make subdirectory for our overlay
mkdir -p /sys/kernel/config/device-tree/overlays/test

#Install overlay
cat /root/overlay/zx5-obru-uio-overlay.dtbo > /sys/kernel/config/device-tree/overlays/test/dtbo
