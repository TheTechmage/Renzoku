function! s:generateList(globlist, matchlist)
	for b in split(globpath('.', a:globlist), '\n')
		if !empty(matchstr(b, a:matchlist))
			execute "normal o\t".b
		endif
	endfor
endfunction
function! Tester()
	set paste
	let a:a="src"
	execute "normal o".a:a
	call s:generateList("*", ".*\.\(cpp\)")
	let a:a="tests"
	execute "normal o".a:a
	call s:generateList("*.cpp", ".*/test_.*\.cpp")
	redraw!
	set nopaste
endfunction
