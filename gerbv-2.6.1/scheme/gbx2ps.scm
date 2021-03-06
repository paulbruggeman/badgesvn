#!/bin/sh
exec guile -l parse-gerber.scm -e main -s "$0" "$@"
!#

; gEDA - GNU Electronic Design Automation
; parse-gerber.scm 
; Copyright (C) 2000-2001 Stefan Petersen (spe@stacken.kth.se)
;
; $Id$
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA


(define *last-aperture-type* '())
(define *last-x* '())
(define *last-y* '())

(define (ps-preamble port)
  (display "%!\n" port)
  (display "%generated by gerbv\n\n" port)
  (display "/inch {72 mul} def\n" port)
  (display "/mm {2.54 div} def\n" port)
  (newline port)
  (display "1.5 inch 3 inch translate\n" port)
  (newline port)
  (if (equal? (assoc-ref *image* 'polarity) 'positive)
      (begin
	(display "/Black {0 setgray} def\n" port)
	(display "/White {1 setgray} def\n" port))
      (begin
	(display "/Black {1 setgray} def % Polarity reversed\n" port)
	(display "/White {0 setgray} def\n" port)))
  (newline port)
  (display "/circle { % x y id od\n" port)
  (display "	gsave\n" port)
  (display "	3 index 3 index moveto\n" port)
  (display "	3 index 3 index 3 2 roll % Fix arguments\n" port)
  (display "	2 div % d given, need r\n" port)
  (display "	0 360 arc Black fill % outer\n" port)
  (display "	2 div % d given, need r\n" port)
  (display "	0 360 arc White fill %inner\n" port)
  (display "grestore\n" port)
  (display "} def\n" port)
  (newline port)
  (display "/rectangle { % x y xl yl\n" port)
  (display "	gsave\n" port)
  (display "	newpath\n" port)
  (display "	1 setlinewidth\n" port)
  (display "	3 index 2 index 2 div sub\n" port)
  (display "	3 index 2 index 2 div add moveto\n" port)
  (display "	1 index 0 rlineto\n" port) ; ->
  (display "	dup -1 mul 0 exch rlineto\n" port) ; \!/
  (display "	1 index -1 mul 0 rlineto\n" port) ; <-
  (display "	dup 0 exch rlineto\n" port) ; /!\
  (display "	pop pop pop pop closepath  Black fill\n" port)
  (display "	grestore\n" port)
  (display "} def\n" port)
  (newline port)
  (set! *max-x* (+ *max-x* 100))
  (set! *max-y* (+ *max-y* 100))
  (set! *min-x* (- *min-x* 100))
  (set! *min-y* (- *min-y* 100))
  (display "gsave 72 setlinewidth newpath\n" port)
  (display (string-append (number->string *max-x*) " 1000 div inch ") port)
  (display (string-append (number->string *max-y*) " 1000 div inch moveto\n") port)
  (display (string-append (number->string *max-x*) " 1000 div inch ") port)
  (display (string-append (number->string *min-y*) " 1000 div inch lineto\n") port)
  (display (string-append (number->string *min-x*) " 1000 div inch ") port)
  (display (string-append (number->string *min-y*) " 1000 div inch lineto\n") port)
  (display (string-append (number->string *min-x*) " 1000 div inch ") port)
  (display (string-append (number->string *max-y*) " 1000 div inch lineto\n") port)
  (display "closepath White fill grestore\n" port)
  (newline port))
	   
(define (print-ps-element element port)
  (let ((x (car element))
	(y (cadr element))
	(aperture-type (car (caddr element)))
	(aperture-state (cdr (caddr element))))
    (cond ((eq? aperture-state 'exposure-off)
	   (handle-line-aperture aperture-type port)
	   (print-position x y port)
	   (display "moveto\n" port))
	  ((eq? aperture-state 'exposure-on)
	   (handle-line-aperture aperture-type port)
	   (print-position x y port)
	   (display "lineto\n" port))
	  ((eq? aperture-state 'exposure-flash)
	   (print-position x y port)
	   (print-flash-aperture aperture-type port)))
    (set! *last-x* x)
    (set! *last-y* y)))

(define (print-position x y port)
  (display x port) ; X axis
  (display " 1000 div " port)
  (display (assoc-ref *image* 'unit) port)
  (display " " port)
  (display y port) ; Y axis
  (display " 1000 div " port)
  (display (assoc-ref *image* 'unit) port)
  (display " " port))

(define (handle-line-aperture aperture-type port)
  (cond ((null? *last-aperture-type*) ; First time
	 (set! *last-aperture-type* aperture-type)
	 (display (get-aperture-size aperture-type) port)
	 (display " inch setlinewidth\n" port))
	((not (eq? *last-aperture-type* aperture-type)) ; new aperture
	 (display "stroke\n" port)
	 (display *last-x* port) ; X Axis
	 (display " 1000 div " port)
	 (display (assoc-ref *image* 'unit) port)
	 (display " " port)
	 (display *last-y* port)
	 (display " 1000 div " port)
	 (display (assoc-ref *image* 'unit) port)
	 (display " moveto\n" port)
	 (display (get-aperture-size aperture-type) port)
	 (display " inch setlinewidth\n" port)
	 (set! *last-aperture-type* aperture-type))))
  
(define (print-flash-aperture aperture-type port)
  (let* ((aperture-description (assv aperture-type *aperture-description*))
	 (symbol (cadr aperture-description))
	 (sizes (cddr aperture-description)))
    (case (length sizes)
      ((1) 
       (display " 0 " port)
       (display (car sizes) port)
       (display " inch " port))
      ((2)
       (display (car sizes) port)
       (display " inch " port)
       (display (cadr sizes) port)
       (display " inch " port)))
    (case symbol
      ((circle)
       (display " circle" port))
      ((rectangle)
;       (display " pop pop moveto " port))
       (display " rectangle  " port))
      (else 
       (display " moveto %unknown" port))))
  (newline port))

(define (get-aperture-size type)
  (let ((desc (assv type *aperture-description*)))
    (if desc
	(caddr desc))))


(define (generate-ps netlist port)
  (ps-preamble port)
  (for-each (lambda (element)
	      (print-ps-element element port))
	    netlist)
  (display "stroke\nshowpage\n" port))

(define *max-x* 0)
(define *max-y* 0)
(define *min-x* 30000)
(define *min-y* 30000)

(define (find-boundaries netlist)
  (if (null? netlist)
      '()
      (let ((x (string->number (caar netlist)))
	    (y (string->number (cadar netlist))))
	(if (< x *min-x*)
	    (set! *min-x* x))
	(if (< y *min-y*)
	    (set! *min-y* y))
	(if (> x *max-x*)
	    (set! *max-x* x))
	(if (> y *max-y*)
	    (set! *max-y* y))
	(find-boundaries (cdr netlist)))))
      
(define (main args)
  (let ((infile 'stdin)    ; this doesn't work
	(outfile "foo.ps")) ; this doesn't work
    (case (length args)
      ((2)
       (set! infile (cadr args)))
      ((3)
       (set! infile (cadr args))
       (set! outfile (caddr args)))
    (else
     (display "Wrong number of arguments.\n    ")
     (display (car args))
     (display " infile [outfile]")
     (newline)
     (exit)))
    (call-with-input-file infile parse-gerber)
    (find-boundaries (reverse *netlist*))
    (call-with-output-file outfile
      (lambda (port) 
	(generate-ps (reverse *netlist*) port)))))
