def half_step_sequence_generator():
  MAX_ALLOWED_ITERATIONS = 4
  iteration_count = 0;
  
  while not iteration_count >= MAX_ALLOWED_ITERATIONS:
    PORTD = 1;
    print(f"FULL={PORTD:02d}, binary=0b{PORTD:04b}")
    while not (PORTD & 1<<3):
      previous_step = PORTD;
      PORTD = (PORTD<<1) | PORTD;
      print(f"HALF={PORTD:02d}, binary=0b{PORTD:04b}")
      PORTD &= ~(previous_step);
      print(f"FULL={PORTD:02d}, binary=0b{PORTD:04b}")
    
    iteration_count += 1
    PORTD |= 1 << 0;
    print(f"HALF={PORTD:02d}, binary=0b{PORTD:04b}")

