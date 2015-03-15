#! /bin/csh
#
# save ckitgui
#
#
if (("$#argv" == "1") & ("$1" == "back")) goto CopyBack
#
#
#------------------------------------------------------------
# copy serv1:BACKUP/gcad3d   -->>  HDEV2:~/devel/gcad3d
echo "===================================="
#echo "sync to /mnt/serv1/Devel/BACKUP"
#rsync -urv --delete --exclude-from=sav_excl.txt\
# /mnt/serv1/Devel/GITHUB/gcad3d\
# /home/fwork/devel
#
if (-e /media/${USER}/DISK/ID) then
echo "===================================="
echo "sync to /media/${USER}/DISK/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/GITHUB/gcad3d\
 /media/${USER}/DISK/BACKUP
endif
#
#
cp -f ~/Schreibtisch/help_git.txt /mnt/serv1/Devel/help/.
goto L_exit
#
#
#------------------------------------------------------------
# copy serv1:BACKUP/gcad3d -> HDEV2:~/devel/gcad3d
CopyBack:
echo "CopyBack:"
#rsync --list-only -urv --exclude-from=sav_excl.txt\
# /mnt/serv1/Devel/BACKUP/gcad3d\
# /home/fwork/devel
# rsync -urv /mnt/serv1/Devel/BACKUP/gcad3d /home/fwork/devel
#
#------------------------------------------------------------
L_exit:
#eof
