" LINUX.    ${DIR_BIN}.vimrc     ${DIR_BIN}.vimrc_orig
" F#; Shift F1 = 23; Ctl-F# geht in Linux nicht !!
" shift <S-F1> Ctrl <C-F1> Alt <M-F1>
" MS: also /mnt/serv1/MSwin/bin/_vimrc


"let opsys = system("uname -s")  geht ned Lifert ein closing oder so ..
let opsys="Linux"
"let myDir = /mnt/serv1/Devel/dev/tools/

" extract filetyp
let FTYP = expand("%:e")
"echo 'FTYP=' FTYP

" else no change-position with mouseclick
:set mouse=n                              " Kommentar notwendig !!!

" used with search (/)
set ignorecase

" history
set hi=20

"undolevels
set ul=100
set cpoptions-=u      " sonst geht undo nur einmal

" anzahl blanks pro tab
set ts=2


let tagfiles = system("ls -m ../tags/*.tag |sed \"s/ //g\"")
"echo tagfiles
let &tags = substitute(tagfiles, "\n", "", "g")

set ruler
"set timeoutlen=100   REMOVED: disables map of 2-char-codes
" sonst shift-select notwendig

" enable Filename im Windowtitle
set title

set noautoindent
set nosmartindent
set nocindent

"------------------------------- COLORS
" 16 Colours-fuers xterm notw.
set t_Co=16
set syntax=c
syntax on
:hi Normal term=NONE ctermfg=15 guifg=#88ff88
:hi Comment term=NONE ctermfg=14 guifg=#40ff40
:hi cComment term=NONE ctermfg=14 guifg=#8888ff
:hi Constant term=NONE ctermfg=13 guifg=#88ff88
:hi PreProc term=NONE ctermfg=11 guifg=#88ff88
:hi Statement term=NONE gui=NONE ctermfg=11
:hi cType term=NONE gui=NONE ctermfg=13 guifg=#8888ff
:hi cStructure term=NONE ctermfg=13 guifg=#88ff88
" fuer :cw ganz wichtig
:hi Search term=bold cterm=NONE ctermfg=10 ctermbg=0
" default dunkelblau(4) = unleserlich
:hi SpecialKey term=bold cterm=NONE ctermfg=6
:hi NonText term=bold cterm=NONE ctermfg=6
:hi Directory term=bold cterm=NONE ctermfg=6
:hi Visual ctermfg=11 ctermbg=8


"-------------------------------- KEYMAPPINGS
" key Ctl-h  show vim-helpfile 
" see ${DIR_BIN}.vimrc_orig  fix vimdoc (Ctrl-h)


" F2 = comment out; insert |// | at first-char-position
if FTYP == "htm"
map <F2> ^^i<!-- $a -->j
else
map <F2> ^^i// j
endif
" Ctl-F2 = comment in; remove first 3 chars
map O1;2Q  ^^xxxj
" Ctrl-F2 = insert |// | ganz links
map O1;5Q  0i// j
" Ctl-Shift-F2 = change comment "/*" or "*/"  into "// .."
map O1;6Q  c2l// .. j0


" Ctl-b - see Bookmark
" bb - see Bookmark
nmap bw :'a,.w! t1


" Ctl-D = Change comment-block // into a doxygen-block ///
map <C-D> :call Doxy_b0()


" Ctl-f = edit file under cursor; geht auch mit gf !
"map ^F :call EDFIL1()
map <C-F> :call JUMP_FILE()


" Ctl-H = display vim-Doku - help
map <C-H> :edit ${HOME}/.vim/.vimdoc
" Alt-H  geht ned !


" Ctl-i / Html - export line, call C-Programm 
if FTYP == "htm"
map <C-I> :call To_Html()
else
" Ctl-i = edit (gleichnamiges) Includefile (Ctl I ist Tab-Key !)
:map <C-I> :call EDINCFIL()
endif
" ?
"map i c5lINT_PTR(i)j


" p print buffer from yy
" Ctl-p = goto Definition (retour m. Ctl o)
map  [
" Alt-p = Display Prototyp from Includefile
map p [i



" q = quit
nmap q :q
" Shift-q   quit mit :q!
" Ctl-Q / Ctl-S  DO NOT USE
"map Q  :q!


" Ctl-R: edit tagobject (Ctrl ])
"map ^R ^]
map <C-R> :call JUMP_TAG()



" Ctl-T = return to previous Tag-File (Ctrl T)


" u     = undo
" Ctl U = redo
map <C-U> :redo


" x remove character - Ctl & Alt do not work



"-------------------------------- ABBREVIATIONS
iab pri printf("\n");k$hhhhi
iab //= //================================================================
iab //- //----------------------------------------------------------------
iab //E //========== EOF =================================================



"-------------------------------- FUNCTIONS

" edit (gleichnamiges) Includefile
:function EDINCFIL ()
"let Fn1 = "echo " . expand("%") . "|cut -f1 -d\".\""
"let Fn2 = system(Fn1)
"let Fn3 = "e " . substitute(Fn2, "\n", ".h", "g")
let Fn1 = "e " . expand("%:r") . ".h"
echo Fn1
execute Fn1
:endfunction


" call C-prog $DIR_BIN"vimTool1(); > -> &lt; < -> &gt;
:function To_Html ()
" write active line -> file ~/.vimLnAct
:.,.w! ~/.vimLnAct
" start C-prog ${DIR_BIN}vimTool1
:silent ! $DIR_BIN"vimTool1"
:redraw!
:.d
:-
" insert file ~/.vimLnAct
:r ~/.vimLnAct
:endfunction


" jump into tag under cursor
:function JUMP_TAG ()
let cmd1 = expand("<cword>")
"echo " func = |". cmd1 ."|"
if cmd1 == ""
let cmd2 = ":vi index.txt"
else
let cmd2 = ":tag " . cmd1
endif
execute cmd2
:endfunction


" jump into file under cursor eg  'xa.c:737: text'
:function JUMP_FILE ()
"let cw = getline(".")
" word under cursor
let cw = expand("<cWORD>")
"echo "word = |" . cw . "|"
" check for ":LineNr"
let v1 = strpart(cw, 0, 1)
"echo "v1 = |" . v1 . "|"
if v1 == ':'
  " die momentane Posi in der Jumplist speichern.
  exe "normal ma"
  exe "normal 'a"
  " jump -> newPos
  exe cw
  "echo " jump -> " . cw
  return
endif
" jump to |filename| or |filename:lNr:|
let cp1 = match(cw,":")
if (cp1 < 0)
 let cmd = ":vi " . expand("<cfile>")
 execute cmd
else
 let fn = strpart(cw,0,cp1)
 let cp1 = cp1 + 1
 let cp2 = match(cw,":",cp1)
 let lw = cp2 - cp1
 let lNr = strpart(cw,cp1,lw)
 let cmd =":vi +" . lNr . " " . fn
 "echo cp1 cp2 lw fn lNr
 "echo cmd
 execute cmd
endif
:endfunction


" Doxygen - Change comment-block // into a doxygen-block ///
" - change a /* */ comment-block into a doxygen-block (///)
:function Doxy_b0 () 
" if 1. line does not have "//": insert "//"; end
" if 1. & 2. line starts with "//": change "//"-block into doxygen-block 
" if 1. starts with "//": insert "/"  ("///")
" erste zeile lesen; test ob sie mit /* beginnt; wenn nein: exit.
let cl1 = getline(".")         " aktuelle Zeile lesen
let cl2 = strpart(cl1, 0, 2)   " extract first 2 chars
"if cl2 == "//" 
"  Doxy_b1
"endif
if cl2 != "/*"
  if cl2 == "//"
    call Doxy_c0()
    return
  endif
  ""echohl WarningMsg
  "echoerr " **** FEHLER **** - cursor muss auf /* stehen !!!"
  " first line ist not /* and not // - insert //; exit
  normal 0i// j
  return
endif
" erste zeile lesen; test ob sie mit */ endet
let ipos = strlen(cl1) - 2
let cl2 = strpart(cl1, ipos, 2)  " die letzen 2 character extracten, testen
if cl2 == "*/"
  normal 0C//================================================================
  return
endif
" change a /* */ comment-block into a doxygen-block (///)
"echo ' func. doxy_b'
" test if cursor is over /* - else exit
while 1 == 1
let cl1 = getline(".")         " aktuelle Zeile lesen
let cl2 = strpart(cl1, 0, 2)   " extract first 2 chars
"echo cl2
if cl2 == "*/"
  normal 0xxi/// \endcodej0
  return
endif
if cl2 == "/*"
  normal 0C//================================================================
  normal o/// \codej
else
  normal 0i/// j
endif
endwhile
:endfunction


:function Doxy_c0 ()
" if 1. & 2. line starts with "//": change "//"-block into doxygen-block 
" if 1. starts with "//": insert "/"  ("///")
" get 2. line
normal j
let cl1 = getline(".")         " aktuelle Zeile lesen
let cl2 = strpart(cl1, 0, 2)   " extract first 2 chars
normal k
if cl2 == "//"
  call Doxy_c1()
  return
endif
normal 0i/j0
:endfunction


:function Doxy_c1 ()
" change a // comment-block into a doxygen-block (///)
normal O/// \codej
while 1 == 1
let cl1 = getline(".")         " aktuelle Zeile lesen
let cl2 = strpart(cl1, 0, 2)   " extract first 2 chars
"echo cl2
if cl2 == "//"
  " test if line already starts with "///"
  let cl2 = strpart(cl1, 0, 3)   " extract first 2 chars
  if cl2 != "///"
    normal 0i/j
  else
    normal j
  endif
else
  normal O/// \endcodej0
  return
endif
endwhile
:endfunction


"----------- bookmarks
:source ~/.vim/vi-bookmarks.vim

" EOF
