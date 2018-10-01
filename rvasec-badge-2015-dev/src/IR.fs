hex
: watchIR
  ." press a key to exit"
  begin
    IRrecv @ ( check recv flag )
    2 = if ( 0 = non, 1 = inprogress, 2 = done )
       IRrecvVal @ u.
       0 IRrecv ! ( mark ready for next )
       0 ( for until )
    else ( see if user wants to exit )
       ?key -1 = ( -1 == empty )
       if
          0 ( for until )
       else 
          drop ( key )
          1 ( for until )
       then
    then
  until
;


