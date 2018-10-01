# Forth Interpreter Processing

## main()

Processing starts in "microchip.c" with function `main()`. That file
includes the intialization code for the processor. Most of the code is
related to setting up USB communications.

`main()` then calls the `ProcessIO()` function in an endless loop.

## ProcessIO()

`ProcessIO()` in "main.c" is the code that's called repeatedly when
the badge is up and running. In addition to calling whichever app is
running at the moment, it handles USB incoming communications

If the first character in a USB message is a "!", turns on forth mode.

    if (USB_In_Buffer[0] == '!') {
        USB_In_Buffer[0] = 0;
        forth_mode = 1;
    }

Next run through `main()` loop, it calls `ForthUSB()`.

    if (forth_mode) {
        ForthUSB() ;
        resume();
        return;
    }

`ForthUSB()` handles sending keys from the USB port to Forth's
`usbkey_buffer` buffer, and writing the contents of the `emit_buffer`
buffer to the USB port.

`resume()` is the primary Forth process, an assembly language routine
defined in file "jonesforth.S".

## ForthUSB()

    /*
      This is the USB call back function when the Forth system
      is in "control"
      so the loop needs to call the "main" function
    */
    void ForthUSB() {
        //  BlinkUSBStatus();

        if (mchipUSBnotReady()) return;

        // if all read, reset buffer
        if (usbkey_fillptr == usbkey_ptr) {
           usbkey_fillptr = 0;
           usbkey_ptr = 0;
        }

        // if not full see if more available
        //if (usbkey_fillptr != MAX_USBKEYBUFFER) {
        if ( (MAX_USBKEYBUFFER - usbkey_fillptr) > 0) {
            int cnt=0;

            if ( (cnt = getsUSBUSART((char *)&(usbkey_buffer[usbkey_fillptr]), (MAX_USBKEYBUFFER - usbkey_fillptr) )) != 0) {
                usbkey_fillptr += cnt;
            }
        }

        // forth key buffer empty?
        if (key_buffer == -1) {
            // any chars available?
            if (usbkey_ptr != usbkey_fillptr) {
                key_buffer = usbkey_buffer[usbkey_ptr];
                usbkey_ptr++;
            }
        }

        // buffer output chars
        if (emit_buffer != -1) {
            USB_Out_Buffer[NextUSBOut++] = emit_buffer;
            emit_buffer = -1;
        }

        if((USBtransferReady()) && (NextUSBOut > 0)) {
            putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
            NextUSBOut = 0;
        }

        CDCTxService();
    }

## resume()

`resume()` handles the transition from C code to Forth code.

### pseudo-code

        if forth_init not called:
            forth_init()
        else:
            restore saved forth environment
        jump to saved Forth routine
        
## forth_init()

`forth_init()` sets up forth variables and registers. It then pushes
those registers onto the Forth stack so that the rest of `resume()`
can use the same code for initialization and resuming from C code.

### Forth Variables

| name               | forth nm | type     | description |
| -------------------------------------------------------|
| data\_segment      | here     | 2K bytes | forth dictionary |
| errBuffer          | errBuf   | 80 bytes | Buffer for error messages  |
| errTable           | errors   | array of int, char * | standard error numbers and message table |
| forth\_stack\_top  |          | address  | top of forth stack, for overflow detection |
| init\_magic        |          | 4 bytes  | flag to indicate forth is initialized |
| var\_HANG          | _        | word     | location of final built-in dictionary item |
| name\_HANG         | LATEST   | address  | marks last dictionary word |
| var\_here          | here     | address  | current location in data memory |
| return\_stack\_top |          | 32 cells | forth return stack |
| var\_base          | base     | int      | current numeric radix |
| var\_errNo         | errNo    | int      | current error number |
| var\_state         | state    | int      | 0 if interpreting, 1 if compiling |
| var_s0             | S0       | address  | pointer to top of stack |
| word\_buffer       |          | char[64] | buffer for read of a word |
| y\_SP              |          | address  | Location to store forth stack pointer when yielding |

### pseudo-code

    forth_init:
	    var_errNo = 0
		var_errors = 0
		var_here = data_segment
        $s4 = return_stack_top   // grows up
        $s5 = forth_stack_top-4  // grows down
		var_s0 = forth_stack_top
		var_base = 10
		var_state = 0	
        init_magic = 0xBEEF
		LBUFPTR = 0	
        //
        // setup save info for first c resume
        //
        init_return = $s8  // this is the initial return address for resume
        
        PUSH    $a0, $a1, $v0, $v1, $s0, $s1, $s2, $s3, $s4, $s6, $s7, $s8
		
        // y_SP == yield stack pointer, for call/return from C app.
        // since $s5 is the stack address and is used in POP/PUSH macros
        // we store it in a variable for c/forth task switching
		y_SP = $s5

## yield

`yield` is a Forth word which returns control from Forth back to the
`ProcessIO()` C code. Any Forth routine which does not exit very
quickly should call it periodically, so that interrupts, USB, IR,
input, etc. handling occurs.

### pseudo-code

    Save Forth environment ($a, $v, and $s registers) on the Forth
        stack (uses $S5 [register 21] as stack pointer).
    Restore C state from C stack (using $sp [register 29] as stack pointer)
    Return from C call (the original resume())
    

