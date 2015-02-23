#! /bin/csh
#
# save ckitgui
#
echo "===================================="
echo "sync to /mnt/serv1/Devel/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /home/fwork/devel/gcad3d /mnt/serv1/Devel/BACKUP
# -n = dryRun, --info=ALL shows all 
#
if (-e /media/${USER}/DISK/ID) then
echo "===================================="
echo "sync to /media/${USER}/DISK/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /home/fwork/devel/gcad3d /media/${USER}/DISK/BACKUP
endif
#
#
#
#------------------------------------------------------------
# BACK:
# rsync -urv /mnt/serv1/Devel/BACKUP/gcad3d /home/fwork/devel
#
cp -f ~/Schreibtisch/help_git.txt /mnt/serv1/Devel/help/.
#
#eof
