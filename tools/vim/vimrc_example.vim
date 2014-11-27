syntax on
let performance_mode=1
let use_plugins_i_donot_use=0
set nocompatible
set history=400
filetype on
filetype plugin on
filetype indent on
set autoread
set mouse=a
let mapleader = ","
let g:mapleader = ","
nmap <leader>x :xa!<cr>
nmap <leader>w :w!<cr>
if has("multi_byte")
    "set bomb
  set fileencodings=ucs-bom,utf-8,cp936,big5,euc-jp,euc-kr,latin1
  " cjk environment detection and corresponding setting
  if v:lang =~ "^zh_cn"
    " use cp936 to support gbk, euc-cn == gb2312
    set encoding=cp936
    set termencoding=cp936
    set fileencoding=cp936
  elseif v:lang =~ "^zh_tw"
    " cp950, big5 or euc-tw
    " are they equal to each other?
    set encoding=big5
    set termencoding=big5
    set fileencoding=big5
  elseif v:lang =~ "^ko"
    " copied from someone's dotfile, untested
    set encoding=euc-kr
    set termencoding=euc-kr
    set fileencoding=euc-kr
  elseif v:lang =~ "^ja_jp"
    " copied from someone's dotfile, untested
    set encoding=euc-jp
    set termencoding=euc-jp
    set fileencoding=euc-jp
  endif
  " detect utf-8 locale, and replace cjk setting if needed
  if v:lang =~ "utf8$" || v:lang =~ "utf-8$"
    set encoding=utf-8
    set termencoding=utf-8
    set fileencoding=utf-8
  endif
endif
if exists("&ambiwidth")
    set ambiwidth=double
endif
if has("gui_running")
    set guioptions-=m
    set guioptions-=T
    nnoremap <silent> <F10> :if &guioptions =~# 'T' <Bar>
        \set guioptions-=T <Bar>
        \set guioptions-=m <Bar>
    \else <Bar>
        \set guioptions+=T <Bar>
        \set guioptions+=m <Bar>
    \endif<CR>
    set guioptions-=l
    set guioptions-=L
    set guioptions-=r
    set guioptions-=R
    " 设置字体
    set guifont=Courier\ New\ 14
    " 设置启动位置和窗口大小
    winpos 235 235
    set lines=25 columns=108
    colorscheme darkblue
else
    colorscheme darkblue
endif
map <leader>1 :set syntax=cheetah<cr>
map <leader>2 :set syntax=xhtml<cr>
map <leader>3 :set syntax=python<cr>
map <leader>4 :set ft=javascript<cr>
map <leader>$ :syntax sync fromstart<cr>
if has("gui_running")
    if exists("&cursorline")
        set cursorline
    endif
endif
set ffs=unix,dos,mac
nmap <leader>fd :se ff=dos<cr>
nmap <leader>fu :se ff=unix<cr>
set so=4
set wildmenu
set ruler
set cmdheight=2
set nu
set lz
set hid
set backspace=eol,start,indent
set whichwrap+=<,>,h,l
set incsearch
set magic
set noerrorbells
set novisualbell
set t_vb=
set showmatch
set mat=8
set hlsearch
if performance_mode
else
    set laststatus=2
    set statusline=
    set statusline+=%2*%-3.3n%0*\ " buffer number
    set statusline+=%f\ " file name
    set statusline+=%h%1*%m%r%w%0* " flags
    set statusline+=[
    if v:version >= 600
        set statusline+=%{strlen(&ft)?&ft:'none'}, " filetype
        set statusline+=%{&encoding}, " encoding
    endif
    set statusline+=%{&fileformat}] " file format
    if filereadable(expand("$VIM/vimfiles/plugin/vimbuddy.vim"))
        set statusline+=\ %{VimBuddy()} " vim buddy
    endif
    set statusline+=%= " right align
    set statusline+=%2*0x%-8B\ " current char
    set statusline+=%-14.(%l,%c%V%)\ %<%P " offset
    if has("autocmd")
        au FileType qf
                    \ if &buftype == "quickfix" |
                    \ setlocal statusline=%2*%-3.3n%0* |
                    \ setlocal statusline+=\ \[Compiler\ Messages\] |
                    \ setlocal statusline+=%=%2*\ %<%P |
                    \ endif
        fun! FixMiniBufExplorerTitle()
            if "-MiniBufExplorer-" == bufname("%")
                setlocal statusline=%2*%-3.3n%0*
                setlocal statusline+=\[Buffers\]
                setlocal statusline+=%=%2*\ %<%P
            endif
        endfun
        if v:version>=600
            au BufWinEnter *
                        \ let oldwinnr=winnr() |
                        \ windo call FixMiniBufExplorerTitle() |
                        \ exec oldwinnr . " wincmd w"
        endif
    endif
    if has('title') && (has('gui_running') || &title)
        set titlestring=
        set titlestring+=%f\ " file name
        set titlestring+=%h%m%r%w " flags
        set titlestring+=\ -\ %{v:progname} " program name
    endif
endif
set switchbuf=usetab
if exists("&showtabline")
    set stal=2
endif
map <leader>cd :cd %:p:h<cr>
vnoremap $1 <esc>`>a)<esc>`<i(<esc>
vnoremap $2 <esc>`>a]<esc>`<i[<esc>
vnoremap $3 <esc>`>a}<esc>`<i{<esc>
vnoremap $$ <esc>`>a"<esc>`<i"<esc>
vnoremap $q <esc>`>a'<esc>`<i'<esc>
vnoremap $w <esc>`>a"<esc>`<i"<esc>
if has("autocmd")
    au BufNewFile,BufRead *.js,*.htc,*.c,*.tmpl,*.css ino $c /**<cr> **/<esc>O
endif
ia xdate <c-r>=strftime("%d/%m/%y %H:%M:%S")<cr>
nmap <M-j> mz:m+<cr>`z
nmap <M-k> mz:m-2<cr>`z
vmap <M-j> :m'>+<cr>`<my`>mzgv`yo`z
vmap <M-k> :m'<-2<cr>`>my`<mzgv`yo`z
func! Cwd()
    let cwd = getcwd()
    return "e " . cwd
endfunc
func! DeleteTillSlash()
    let g:cmd = getcmdline()
    let g:cmd_edited = substitute(g:cmd, "(.*[\]).*", "", "")
    if g:cmd == g:cmd_edited
        let g:cmd_edited = substitute(g:cmd, "(.*[\]).*[\]", "", "")
    endif
    return g:cmd_edited
endfunc
func! CurrentFileDir(cmd)
    return a:cmd . " " . expand("%:p:h") . "/"
endfunc
set viminfo='10,"100,:20,%,n~/.viminfo
set nobackup
set nowb
set noswapfile
if exists("&foldenable")
    set fen
endif
if exists("&foldlevel")
    set fdl=0
endif
set expandtab
set shiftwidth=4
set softtabstop=4
set tabstop=4
set backspace=2
set smarttab
set lbr
set ai
set si
if has("cindent")
    set cindent
endif
set wrap
if has("autocmd") && v:version>600
    au BufRead,BufNew *.vim map <buffer> <leader><space> :w!<cr>:source %<cr>
endif
let xml_use_xhtml = 1
let html_use_css = 0
let html_number_lines = 0
let use_xhtml = 1
if has("eval") && has("autocmd")
    "vim 5.8.9 on mingw donot know what is <SID>, so I avoid to use function
    "c/cpp
    fun! Abbrev_cpp()
        ia <buffer> cci const_iterator
        ia <buffer> ccl cla
        ia <buffer> cco const
        ia <buffer> cdb bug
        ia <buffer> cde throw
        ia <buffer> cdf /** file<CR><CR>/<Up>
        ia <buffer> cdg ingroup
        ia <buffer> cdn /** Namespace <namespace<CR><CR>/<Up>
        ia <buffer> cdp param
        ia <buffer> cdt test
        ia <buffer> cdx /**<CR><CR>/<Up>
        ia <buffer> cit iterator
        ia <buffer> cns Namespace ianamespace
        ia <buffer> cpr protected
        ia <buffer> cpu public
        ia <buffer> cpv private
        ia <buffer> csl std::list
        ia <buffer> csm std::map
        ia <buffer> css std::string
        ia <buffer> csv std::vector
        ia <buffer> cty typedef
        ia <buffer> cun using Namespace ianamespace
        ia <buffer> cvi virtual
        ia <buffer> #i #include
        ia <buffer> #d #define
    endfunction
    fun! Abbrev_java()
        ia <buffer> #i import
        ia <buffer> #p System.out.println
        ia <buffer> #m public static void main(String[] args)
    endfunction
    fun! Abbrev_python()
        ia <buffer> #i import
        ia <buffer> #p print
        ia <buffer> #m if __name__=="__main__":
    endfunction
    fun! Abbrev_aspvbs()
        ia <buffer> #r Response.Write
        ia <buffer> #q Request.QueryString
        ia <buffer> #f Request.Form
    endfunction
    fun! Abbrev_js()
        ia <buffer> #a if(!0){throw Error(callStackInfo());}
    endfunction
    augroup abbreviation
        au!
        au FileType javascript :call Abbrev_js()
        au FileType cpp,c :call Abbrev_cpp()
        au FileType java :call Abbrev_java()
        au FileType python :call Abbrev_python()
        au FileType aspvbs :call Abbrev_aspvbs()
    augroup END
endif
map < :%s/s*$//g<cr>:noh<cr>''
ino <C-v> <esc>:set paste<cr>mui<C-R>+<esc>mv'uV'v=:set nopaste<cr>

" 中文帮助
set helplang=cn

" tab标签页操作
nmap tn :tabnew 
nmap tc :tabclose<CR>
nmap tm :tabmove
nmap th :tabprev<CR>
nmap tl :tabnext<CR>
nmap ty :tabonly<CR>
" 分裂窗口
" 竖直窗口
nmap tv :vsplit 
" 水平窗口
nmap to :split 
" 去掉^M符号
" nmap cm :%s/<C-v><C-m>//g<CR>
noremap <leader>m :%s/\r//g<CR>
" 关闭快捷键
nmap <leader>q :q!<CR>
" 设置空格来开关折叠
set foldenable
set foldmethod=indent
nnoremap <space> @=((foldclosed(line('.'))<0)?'zc':'zo')<CR>
" 窗口中跳转的快捷方式
nmap wl <C-w>l
nmap wh <C-w>h
nmap wj <C-w>j
nmap wk <C-w>k
" 关闭窗口
nmap wc <C-w>c
" 竖直窗口打开当前文件
nmap wv :vsplit<CR>
" 水平窗口打开当前文件
nmap wo :split<CR>
" VIM自带的补全
imap <M-i> <C-p>
