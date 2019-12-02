" LINUX.    ~/.vimrc
" /bin/cp -f ~/.vimrc ${DIR_UTI}vim/.
" /bin/cp -f ~/.vimrc ${DIR_DEV}gcad3d/src/APP/.
" /bin/cp -f ~/.vimdoc ${DIR_UTI}vim/.
" /bin/cp -f ~/.vimdoc ${DIR_DEV}gcad3d/src/APP/.
" F#; Shift F1 = 23; Ctl-F# geht in Linux nicht !!
" shift <S-F1> Ctrl <C-F1> Alt <M-F1>
" MS: also /mnt/serv1/MSwin/bin/_vimrc
" 2019-09-18 dispay htm, jpg added. RF.


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

" temp-usage-only !
map t :call TestFunc1()
map T :call TestFunc2()


" F2 = comment out; insert |// | at first-char-position
if FTYP == "htm"
map <F2> ^^i<!-- $a -->j
else
map <F2> ^^i// j
endif

" Shift-F2 = comment in; remove first 3 chars
map [1;2Q ^^xxxj

" Ctrl-F2 = insert |// | ganz links
map [1;5Q  0i// j

" Ctl-Shift-F2 = change comment "/*" or "*/"  into "// .."
map [1;6Q c2l// .. j0


" Ctl-b - see Bookmark
" bb - see Bookmark
nmap bw :'a,.w! t1


map <C-E> :call EDIT_FILE()


" Ctl-D = Change comment-block // into a doxygen-block ///
map <C-D> :call Doxy_b0()


" Ctl-f = edit file under cursor; geht auch mit gf !
"map ^F :call EDFIL1()
map <silent> <C-F> :call JUMP_FILE()


" Ctl-H = display vim-Doku - help
map <C-H> :edit ${HOME}/.vimdoc
" Alt-H  geht ned !


" Ctl-I cannot be used (Tab)?
" Ctl-i / Html - export line, call C-Programm 
if FTYP == "htm"
nnoremap <silent> bh :call To_Html()
else
" bh = edit (gleichnamiges) Includefile (Ctl I ist Tab-Key !)
nnoremap <silent> bh :call EDINCFIL()
endif


" p print buffer from yy
" Ctl-p = goto Definition of struct (retour m. Ctl o)
map <C-P> :call To_proto()
" Alt-p = Display Prototyp from Includefile
map p [i



" q = quit
map q :q
" Shift-q   quit mit :q!
" Ctl-Q / Ctl-S  DO NOT USE
map Q  :q!


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
iab TB     // TESTBLOCK<CR>    // END TESTBLOCK


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


" jump-to-definition (prototype) - vi -t <word_under_cursor>
:function To_proto ()
let cmd2 = ":tag " . expand("<cword>")
execute cmd2
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


" Ctrl-E: edit into file under cursor eg  'xa.c:737: text'
:function EDIT_FILE ()
 "
 " get cw = word under cursor
 let cw = expand("<cWORD>")
 "echo "word = |" . cw . "|"
 "
 " get fTyp = filetype
 let cp1 = strridx(cw,".")
 if cp1 > 0
  let cp1 = cp1 + 1
  let fTyp = strpart(cw,cp1)
  "echo "fTyp = " . fTyp
  "
  "
  if (strpart(fTyp,0,3) == "htm")
   :let f = system('bluefish ' . cw . " &")
   return
  endif
 endif
:endfunction


" Ctrl-F: jump into file under cursor eg  'xa.c:737: text'
:function JUMP_FILE ()
"let wdc = expand("<cWORD>") - limits with ' ' only
"let wc = expand("<cword>")  " removes '.ftyp'
let wc = expand("<cfile>")   " removes ':'
"echo "wc = " . wc

" test if it starts with '#' = htm-tag
let w1c = strpart(wc, 0, 1)
"echo "w1c = " . w1c
if w1c == '#'
  " htm-tag: add active filename
  let  wdc = expand("%") . wc
  call DispHtm (wdc)
  return
endif

" get filetype
let cp1 = strridx(wc,".")

if cp1 > 0
  let cp1 = cp1 + 1
  let fTyp = strpart(wc,cp1)
   "echo "fTyp = " . fTyp
  
  " test 'fn.htm'
  if(strpart(fTyp,0,3) == "htm")
    " display htm-files with firefox
    call DispHtm (wc)
    return

  " display .jpg with eog
  elseif fTyp == "jpg"
   :let f = system('eog ' . wc . " &")
   return

  " display .png with eog
  elseif fTyp == "png"
    :let f = system('eog ' . wc . " &")
    return
  endif
endif

" <cfile> did remove ':lineNr' - find it
let sc = expand("<cWORD>") " limits with ' ' only
"echo "sc = |" . sc . "| wc = |" . wc . "|"

" jump to |:lNr|
if strpart(sc, 0, 1) == ':'
  " die momentane Posi in der Jumplist speichern.
  exe "normal ma"
  exe "normal 'a"
  " jump -> newPos
  exe sc
  "echo " jump -> " . sc
  return
endif

" check for |filename:lNr|
let cp2 = match(sc,":")
if (cp2 >= 0)
  " extract lNr from sc
  let cp2 = cp2 + 1
  let lNr = strpart(sc,cp2)
  " jump to |filename:lNr|
  let cmd = ":vi +" . lNr . " " . wc
  "echo cmd
  execute cmd
  return
endif

" jump to |filename|
let cmd = ":vi " . wc
execute cmd
return

:endfunction


:function DispHtm (fnam)
"echo "DispHtm |" . a:fnam . "|"
  :let f = system('firefox -P none ' . a:fnam . ' &')
:endfunction


:function GetWord ()
 " returns word under cursor
 let wdc = expand("<cWORD>")
 " remove following '\'
 let wc = substitute(wdc, '\\', '', 'g')
 return wc
:endfunction


"-------------------------------------------------------------------
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


"-------------------------------------------------------------------
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


"-------------------------------------------------------------------
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


"-------------------------------------------------------------------
:function TestFunc2 ()
" change prev line to "================================================================== \endcode */}"
" change active line to "void INF_<word> (){        /*! \code
  " set to start of word
  call SetPosWordStart()
  " copy word
  let suc = expand("<cWORD>")
  " delete 2 lines
  normal kdddd
  let cmd1 = "normal ko" . "================================================================== \\endcode \*\/}"
  execute cmd1
  let cmd1 = "normal o" . "void INF_" . suc . " (){        /*! \code"
  execute cmd1
:endfunction


"-------------------------------------------------------------------
:function TestFunc1 ()
"change <word>  into INF_<word>
  " set to start of word
  call SetPosWordStart()
  " <word> -> void INF_<word>
  :set iskeyword=" "
  " if word starts with "INF_" - do nothing
  let suc = expand("<cWORD>")
  if strpart(suc, 0, 4) != "INF_"
    " change word-delimiters to only blank
    let cwd = "normal cw" . "INF_" . expand("<cexpr>")
    execute cwd
  endif
  " reset word-delimiters
  :set iskeyword&
:endfunction


"-------------------------------------------------------------------
" SetPosWordStart = set cursor to start of word; do NOT change to upper line
:function SetPosWordStart ()
  :set iskeyword=" "
  " if cursorspalte > 1 -
  if col(".") > 1
    " if left-character != ' '
    normal h
    " get char-under-cursor as @ with yl
    normal yl
    let cuc = @
    " go 1 char right
    normal l
    if cuc != ' '
      " goto start of word
      normal b
    endif
  endif
  :set iskeyword&
:endfunction


"-------------------------------------------------------------------
if filereadable(expand("~/.vim/vi-bookmarks.vim"))
:source ~/.vim/vi-bookmarks.vim
endif


"------------------------------- vim-bkmTool
":highlight bkmToolLtab ctermbg=darkred guibg=darkred
":autocmd BufEnter * :call bkmTool#BkmHili()
"nmap bb :call bkmTool#BkmToggle()
"nmap <C-B> :call bkmTool#BkmShow()


" EOF
