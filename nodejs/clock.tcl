 proc every {ms body} {
    eval $body
    after $ms [list every $ms $body]
 }
 pack [label .l -textvar time -font {Tahoma 24}]
 pack [button .b -text X -command exit]
 every 1000 {set ::time [clock format [clock sec] -format %H:%M:%S]}