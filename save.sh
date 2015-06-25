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

endif
#
echo "***** unknown option, use:"
echo "Stick              serv1:GITHUB/gcad3d  -->> STICK:~/BACKUP/gcad3d"
echo "Local              serv1:GITHUB/gcad3d  -->> HDEV2:~/devel/gcad3d"
echo "Forw               HDEV2:~/devel/gcad3d -->> serv1:GITHUB/gcad3d"
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
 /mnt/serv1/Devel/GITHUB\
goto L_exit
#
#
#
#------------------------------------------------------------
# copy serv1:GITHUB/gcad3d   -->>  STICK:~/BACKUP/gcad3d
Stick:
if (! -e /media/${USER}/DISK/ID) then
  echo "***** Stick nicht gemountet .."
  goto L_exit
endif
echo "sync to /media/${USER}/DISK/BACKUP"
rsync -urv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/GITHUB/gcad3d\
 /media/${USER}/DISK/BACKUP
#
cp -f ~/Schreibtisch/help_git.txt /mnt/serv1/Devel/help/.
goto L_exit
#
#
#
#------------------------------------------------------------
# copy serv1:GITHUB/gcad3d -> HDEV2:~/devel/gcad3d
Local:
echo "Local:"
rsync -rv --delete --exclude-from=sav_excl.txt\
 /mnt/serv1/Devel/GITHUB/gcad3d\
 /home/fwork/devel
#
#
#
#------------------------------------------------------------
L_exit:
#eof
