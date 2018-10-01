!

1 IRled
0 IRled
0 IRled
0 IRled
1 IRled

: pause  ( n -- )
  0 begin ( n i=0 -- )
    1+    ( n i+1 -- )
    over over ( n i+1 n i+1 -- )
    =       ( n i+1 0/1 -- )
  until
  drop
  drop
;


hex
FACE IRsendVal !
1 IRsend !

10000 pause

hex
BEEF IRsendVal !
1 IRsend !

IRsend @ u.

hex
IRrecv @ u.
IRrecvVal @ u.
0 IRrecvVal !
0 IRrecv !


