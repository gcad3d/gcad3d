#! /bin/csh
#
#
if ("$#argv" == "1") then

  if ("$1" == "Forw") then
    goto Forw
  endif

  if ("$1" == "Local") then
    goto Local
  endif

  if ("$1" == "Stick") then
    goto Stick
  endif

  if ("$1" == "backup") then
    goto backup
  endif

endif
#
echo "***** unknown option, use:"
echo "Stick              serv1:gcad3d  -->> STICK:~/BACKUP/gcad3d"
echo "Local              serv1:gcad3d  -->> HDEV3:~/devel/gcad3d"
echo "backup             ~/devel/gcad3d -->> ~/backup/gcad3d"
echo "Forw               HDEV2:~/devel/gcad3d -->> serv1:gcad3d"
echo "serv1: = /mnt/serv1/Devel/"
goto L_exit
#
#
#
#------------------------------------------------------------
# copy HDEV2:~/devel/gcad3d  -->>  serv1:GITHUB/gcad3d
Forw:
echo "===================================="
#echo "sync to /mnt/serv1/Devel/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /home/fwork/devel/gcad3d\
 /mnt/serv1/Devel\
goto L_exit
#
#
#
#------------------------------------------------------------
# copy serv1:gcad3d   -->>  STICK:~/BACKUP/gcad3d
Stick:
if (! -e /media/${USER}/DISK/ID) then
  echo "***** Stick nicht gemountet .."
  goto L_exit
endif
echo "sync to /media/${USER}/DISK/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/gcad3d\
 /media/${USER}/DISK/BACKUP
#
#cp -f ~/Schreibtisch/help_git.txt /mnt/serv1/Devel/help/.
goto L_exit
#
#
#
#------------------------------------------------------------
# copy serv1:/Devel/gcad3d -> HDEV2:~/devel/gcad3d
Local:
echo "Local:"
echo "/mnt/serv1/Devel/gcad3d  =>   /home/fwork/devel/gcad3d"

# update after reLoading from github:
#rsync -rv --ignore-times --size-only --delete --exclude-from=sav_excl.txt\

# normal update:
rsync -urv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/gcad3d\
 /home/fwork/devel
#
goto L_exit
#
#
#------------------------------------------------------------
# copy serv1:/Devel/gcad3d -> ~/backup/gcad3d
backup:
echo "Local:"
echo "/mnt/serv1/Devel/gcad3d  =>   /home/fwork/devel/gcad3d"

rsync -urv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/gcad3d\
 /home/fwork/backup
#
goto L_exit
#
#
#------------------------------------------------------------

L_exit:
#eof
