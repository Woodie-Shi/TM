; This program checks if the length of the input string is a square number.
; Input: a string of 1's, length belongs to N, e.g. '1111'

; the finite set of states
#Q = {0,move_right,add_to,move_left,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 1_ 11 ** move_right
0 __ __ ** accept

; State move_right: 2n+1's 1 
move_right 11 _1 rr move_right
move_right 1_ 11 *r add_to
move_right __ __ ** accept
move_right _1 __ ** reject

; State add_to: add_to a 1 at the end of the tape
add_to 1_ 11 *l move_left

; State move_left: move back to the head
move_left 11 11 *l move_left
move_left 1_ 1_ *r move_right

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject ** __ rl reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject