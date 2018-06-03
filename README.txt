Andriod Marshmallow Porting on Beaglebone Black and Driver development
======================================================================

1. Prerequisite: 
   ============

Older versions of Android require a separate installation of the JDK. 

For Ubuntu >= 15.04 >>>>

follow : https://askubuntu.com/questions/67909/how-do-i-install-oracle-jdk-6?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

Installing required packages (Ubuntu 14.04)

You will need a 64-bit version of Ubuntu. Ubuntu 14.04 is recommended. 
 
sudo apt-get install git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev libgl1-mesa-dev libxml2-utils xsltproc unzip


#----------------------------------------------------------------------------------------------------------------------------------------------
Before you get started, you should make sure that you are using a 64-bit machine and have at least 45 GB of available disk space to hold the BBBAndroid source, intermediate build files, and final images. You'll also need 6 to 8 GB of RAM to accommodate the memory requirements of the build process. Once you've verified that you meet these requirements, it is time to get started.

Download the repo tool and install it somewhere convenient in your system where it will be in your path:

$ mkdir ~/bin
$ PATH=~/bin:$PATH
$ curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
$ chmod a+x ~/bin/repo

You could also copy repo to /usr/local/bin or the like, though I would discourage you from doing so.

#----------------------------------------------------------------------------------------------------------------------------------------------
Building BBBAndroid
===================
Create a directory to build BBBAndroid in. This will be the "root" of the build: 
For my case :
#mkdir /root/bbbandroid

 Change into the root and clone the BBBAndroid source code:

# cd bbbandroid
# repo init -u http://github.com/hendersa/bbbandroid-manifest
# repo sync -c 

Depending upon how fast your internet connection speed is, this can take a several hours to complete. Currently, 361 different git repositories are downloaded to construct the entire BBBAndroid source codebase. These repositories require about 18 GB of disk space. Once the code has downloaded, run the patch.sh script. This script will patch a few files belonging to the AOSP repos.
t is time to begin building BBBAndroid. Setup the environment of your shell for building and then configure the build to use the BeagleBone Black device files:

# . build/envsetup.sh
# lunch beagleboneblack-eng

Then, begin the build with make. The -jX option for make will build X files concurrently. Even if you only have a single-core CPU, you should use at least -j2 (as much of the build process is I/O-bound, rather than CPU bound). If you want to use a much higher X for your build, go for it. The more resources (RAM and CPU cores) that you have, the more compilation processes that you can execute concurrently and the shorter the build process will be.

$ make -j4

Towards the end of the build process, the kernel will begin building and request some input:

* Restart config...
*
*
* USB Gadget Support
*
USB Gadget Support (USB_GADGET) [Y/n/m/?] y
  Debugging messages (DEVELOPMENT) (USB_GADGET_DEBUG) [N/y/?] n
  Debugging information files (DEVELOPMENT) (USB_GADGET_DEBUG_FILES) [Y/n/?] y
  Debugging information files in debugfs (DEVELOPMENT) (USB_GADGET_DEBUG_FS) [Y/n/?] y
  Maximum VBUS Power usage (2-500 mA) (USB_GADGET_VBUS_DRAW) [2] 2
  Number of storage pipeline buffers (USB_GADGET_STORAGE_NUM_BUFFERS) [2] 2
  USB Gadget Drivers [M/y/?] (NEW)

Type "y" and then hit enter. This menu will pop up:

USB Gadget Drivers
    1. Gadget Zero (DEVELOPMENT) (USB_ZERO)
    2. Audio Gadget (USB_AUDIO)
    3. Ethernet Gadget (with CDC Ethernet support) (USB_ETH)
    4. Network Control Model (NCM) support (USB_G_NCM)
    5. Gadget Filesystem (USB_GADGETFS)
  > 6. Function Filesystem (USB_FUNCTIONFS)
    7. Mass Storage Gadget (USB_MASS_STORAGE)
    8. Serial Gadget (with CDC ACM and CDC OBEX support) (USB_G_SERIAL)
    9. MIDI Gadget (USB_MIDI_GADGET)
    10. Printer Gadget (USB_G_PRINTER)
    11. CDC Composite Device (Ethernet and ACM) (USB_CDC_COMPOSITE)
    12. Nokia composite gadget (USB_G_NOKIA)
    13. CDC Composite Device (ACM and mass storage) (USB_G_ACM_MS)
    14. Multifunction Composite Gadget (USB_G_MULTI)
    15. HID Gadget (USB_G_HID)
    16. EHCI Debug Device Gadget (USB_G_DBGP)
    17. USB Webcam Gadget (USB_G_WEBCAM)
  choice[1-17?]: 

Just hit enter. The build will continue on. Once the build has finally completed, it is time to build the necessary Android file systems. This portion of the BBBAndroid build process is based upon the build scripts developed for 
 Change into the root and clone the BBBAndroid source code:

#make sdcard_build

Finally, it is time to create the final microSD card image. Four partitions will be created on the card: boot (which contains the kernel and bootloader), rootfs (which contains the "system" libraries, binaries, and .rc files), usrdata (data stored "on" the Android device), and data (data stored "off" the Android device on external storage).


Please create 4 partition manually, which is mentioned bellow :
-------------------------------------------------------------
 Device     Boot   Start      End Sectors  Size Id Type
/dev/sdd1  *         63   144584  144522 70.6M  c W95 FAT32 (LBA)
/dev/sdd2        144585  4016249 3871665  1.9G 83 Linux
/dev/sdd3       4016250  7887914 3871665  1.9G 83 Linux
/dev/sdd4       7887915 14905343 7017429  3.4G  c W95 FAT32 (LBA)

Comment line 69 of mkmmc-android.sh file 

#dd if=/dev/zero of=$DRIVE bs=1024 count=1024 &>/dev/null 

Rowboat script named mkmmc-android.sh will partition your microSD card and build the file systems for you. Insert a microSD card into your build machine. Before you continue, determine what device on your system is your microSD card. On my system, it is /dev/sdb (/dev/sda is the hard drive). But, it may be different on yours. Make sure that you know for certain that you have identified the proper device, since that device will be wiped clean by the next commands:

# cd out/target/product/beagleboneblack/beagleboneblack/
# sudo ./mkmmc-android.sh [YOUR DEVICE FILE FOR THE MICROSD CARD (i.e. /dev/sdb)]

ssuming Default Locations for Prebuilt Images
All data on [YOUR DEVICE FILE] now will be destroyed! Continue? [y/n]

Type "y" and hit enter. The script will create a new partition table, partitions, and file systems. It will then copy all of the necessary files into place in the new partitions. Unmount the microSD card (if your system automounted it after writing), insert the card into your BBB, power on the BBB, and BBBAndroid will now be running. 

Ref : http://bbbandroid.sourceforge.net/build.html
#----------------------------------------------------------------------------------------------------------------------------------------------

Driver development on Android Platform :
=======================================

#vim mfd.c
----------

#include <linux/module.h>
#include <linux/init.h>

int myvar;

int test_fun(int param1)
{
        pr_info("%s: param value is %d\n",__func__,param1);
        return 0;
}
int myinit(void){
        printk("%s: My first device driver\n",__func__);
        myvar=10;
        return 0;
}

void myexit(void){
        printk("%s: Good Bye.!\n",__func__);
}


EXPORT_SYMBOL_GPL(myvar);
EXPORT_SYMBOL_GPL(test_fun);

module_init(myinit);
module_exit(myexit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("xyz");
MODULE_DESCRIPTION("My first driver");

#-------------------------------------------------------------------------------------#
#vim Makefile

obj-m := mfd.o

KDIR =  /root/bbbandroid/kernel

PWD := $(shell pwd)

default:
        $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
        $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

#make ARCH=arm CROSS_COMPILE=arm-linux-androideabi- CFLAGS_MODULE=-fno-pic

#-------------------------------------------------------------------------------------#
#Set Tool Chain path  
#vim ~/.bashrc
PATH=$PATH:/root/bbbandroid/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.7/bin
. ~/.bashrc
#-------------------------------------------------------------------------------------#
#Driver compilation and Driver load 
make ARCH=arm CROSS_COMPILE=arm-linux-androideabi- CFLAGS_MODULE=-fno-pic
After compilation you will get mfd.ko

#connect adb #
#adb push mfd.ko /
#adb shell 
#insmod mfd.ko
<4>[ 2171.599515] myinit: My first device driver
