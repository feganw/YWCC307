* Integral benchmark adapted from https://cboard.cprogramming.com/c-programming/47759-mac-os-x-users-c-programmers.html
      PROGRAM integral
* Declarations
	  REAL isum, delta, x, y, z, t_start, t_stop
	  INTEGER i, j, k
	  isum = 0.0
	  delta = 0.025
	  x = -5.0
	  
	  WRITE (*,*) '-----------'
	  WRITE (*,*) '* FORTRAN *'
	  WRITE (*,*) '-----------'
	  WRITE (*,*) 'Approximation of a triple integral over a ball.'
	  
* Need to begin clock here.
      call CPU_TIME(t_start)

      DO 10 i=0,400
	     x = x + delta
		 y = -5.0
		 DO 15 j=0,400
		    y = y + delta
			z = -5.0
			DO 20 k=0,400
			   z = z + delta
			   IF ((x*x+y*y+z*z).LT.25.0) THEN
			      isum = isum + EXP(-x*x)*COS(y*y)*COS(z*z)
			   ENDIF
   20		CONTINUE
   15	 CONTINUE
   10 CONTINUE
      isum = isum*delta*delta*delta
* print execution time.
      call CPU_TIME(t_stop)
	  WRITE (*,*) 'integral = ', isum
	  WRITE (*,*) 'elapsed time: ', t_stop - t_start, ' (s)'	  
      END	  