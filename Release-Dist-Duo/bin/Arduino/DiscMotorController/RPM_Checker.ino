volatile long pulses = 0 ;  // must be volatile if used in ISR

void int_func ()
{
  pulses ++ ;
}

long get_pulsecount ()  // safely read pulses when its not changing
{
  noInterrupts () ;
  long res = pulses ;  // don't zero the count, just sample it, less scope for bugs (easy debug)
  interrupts () ;
  return res ; 
}




