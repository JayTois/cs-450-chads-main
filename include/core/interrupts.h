#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

typedef struct Context{
  //the stack registers should be in this order
  uint32_t ds;
  uint32_t es;
  uint32_t fs;
  uint32_t gs;
  uint32_t esp;

} Context;

/*
  Procedure..: init_irq
  Description..: Installs the initial interrupt handlers for
      the first 32 irq lines. Most do a panic for now.
*/
void init_irq(void);

/*
  Procedure..: init_pic
  Description..: Initializes the programmable interrupt controllers
      and performs the necessary remapping of IRQs. Leaves interrupts
      turned off.
*/
void init_pic(void);

#endif
