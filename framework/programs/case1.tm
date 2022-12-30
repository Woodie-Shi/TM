; This program rotates a binary number right.
; Input : a string of 0's and 1's, e.g. '11101001'.
; Output: the result, e.g. '11110100'. 

; the finite set of states
#Q = {0,read0,read1,head0,head1,halt_accept}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 1

; the transition functions

; State 0: start state
0 0 _ r read0
0 1 _ r read1

; State read0: record the pre bit is 0
read0 0 0 r read0
read0 1 0 r read1
read0 _ _ l head0

; State read1: record the pre bit is 1
read1 0 1 r read0
read1 1 1 r read1
read1 _ _ l head1

; State head0: record the pre bit is 0, move to head and write
head0 0 0 l head0
head0 1 1 l head0
head0 _ 0 * halt_accept

; State head1: record the pre bit is 1, move to head and write
head1 0 0 l head1
head1 1 1 l head1
head1 _ 1 * halt_accept