#pragma once

#define MMIO_BASE       0x1C000000
#define PL111_CLCD_BASE (MMIO_BASE + 0x20000)
#define CLCD_TIM0       (PL111_CLCD_BASE + 0x000) 
#define CLCD_TIM1       (PL111_CLCD_BASE + 0x004) 
#define CLCD_TIM2       (PL111_CLCD_BASE + 0x008) 
#define CLCD_TIM3       (PL111_CLCD_BASE + 0x00C) 
#define CLCD_UBAS       (PL111_CLCD_BASE + 0x010) 
#define CLCD_CTRL       (PL111_CLCD_BASE + 0x018) 

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define SCREEN_DEPTH  32

volatile u32* framebuffer;

static void init_framebuffer() {
  framebuffer = (volatile u32*)0x30000000;
  *(volatile u32*)CLCD_TIM0
}

