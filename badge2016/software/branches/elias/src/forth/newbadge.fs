!
\ the exclaimation on the first line
\ puts the badge into forth mode
\ load this code using something like this:
\ cat newbadge.fs > /dev/ttyACM0
\ this sends the whole file to ACM0

\ 
\ some docs on badge
\ 
\ RC9 backlight

\ hex

\ BF886000 constant ANSELA 
\ BF886010 constant TRISA  
\ BF886020 constant PORTA  
\ BF886030 constant LATA   

\ BF886100 constant ANSELB 
\ BF886110 constant TRISB  
\ BF886120 constant PORTB  
\ BF886130 constant LATB   

\ BF886200 constant ANSELC 
\ BF886210 constant TRISC  
\ BF886220 constant PORTC  
\ BF886230 constant LATC   


hex
BF886030 constant LATA   
BF886130 constant LATB   
BF886230 constant LATC   

: binary 2 base ! ;


( bit 3 of port c )
binary
: getButton
   ( 00000000001000 trisc ! ) ( configure as input )
   PORTC @ 00000000001000 and 
;

( pin RC9 )
binary
( 1/0 -- ) ( turn backlight on/off )
: backlight
  if
     LATC @ 00001000000000 or LATC ! 
  else
     LATC @ 00001000000000 invert and LATC !
  then
;

( RB13 )
binary
( 1/0 -- ) ( turn IR on/off )
: IRled
  if
     LATB @ 10000000000000 or LATB ! 
  else
     LATB @ 10000000000000 invert and LATB !
  then
;

( RA9 )
binary
: spk
  if
     LATA @ 00001000000000 or LATA !
  else
     LATA @ 00001000000000 invert and LATA !
  then
 ;
( RA9 )

( alt pin rc4 )
\ : spk
\  if
\     LATC @ 00000000010000 or LATC !
\  else
\     LATC @ 00000000010000 invert and LATC !
\  then
\ ;

( LED )
( RC0 )
( RC1 )
( RB3 )

decimal
( 1/0 -- ) \ turn led on/off
: red
  if
     LATC @ 1 or LATC !
  else
     LATC @ 1 invert and LATC !
  then
;

: blue
  if
     LATC @ 2 or LATC !
  else
     LATC @ 2 invert and LATC !
  then
;

: green
  if
     LATB @ 8 or LATB !
  else
     LATB @ 8 invert and LATB !
  then
;


: pause  ( n -- )
  0 begin ( n i=0 -- )
    1+    ( n i+1 -- )
    over over ( n i+1 n i+1 -- )
    =       ( n i+1 0/1 -- )
  until
  drop
  drop
;


( real time clock )
hex
15030501 date !
21520000 time !
setRTCC
setupRTCC ( this takes a while )
getRTCC time @ u.
getRTCC date @ u.

0 backlight
1 backlight


1 spk 1 pause 0 spk 1 pause
1 spk 10 pause 0 spk 10 pause
1 spk 20 pause 0 spk 20 pause
1 spk 50 pause 0 spk 50 pause
1 spk 500 pause 0 spk 500 pause
1 spk 1000 pause 0 spk 1000 pause
1 spk 2000 pause 0 spk 2000 pause
1 spk 5000 pause 0 spk 5000 pause 

0 red 0 green 0 blue


decimal
: watchTouch
  begin
    dup getTouch
    binary c@ u. ( button status )
    hex 8 dump ( averages )

    ?key -1 =
    if
       0 ( for until )
    else
       key dup
       32 = 
       if 
          drop ( key )
          1 ( for until )
       else
          emit ( key )
          0 ( for until )
       then
    then
  until
;

: c LCDcontrast ;

: bob LCDdrbob ;

\ bob

\ 50 c

\ 5 watchTouch

\ LCDReset
\ LCDinit

\ LCDReset
\ LCDinit

