/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2022
 * Generated linker script file for LPC845
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13] on 2 dic. 2022 10:30:30
 */

MEMORY
{
  /* Define each memory region */
  MFlash64 (rx) : ORIGIN = 0x0, LENGTH = 0x10000 /* 64K bytes (alias Flash) */  
  RamLoc16 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x4000 /* 16K bytes (alias RAM) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash64 = 0x0  ; /* MFlash64 */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_MFlash64 = 0x0 + 0x10000 ; /* 64K bytes */  
  __top_Flash = 0x0 + 0x10000 ; /* 64K bytes */  
  __base_RamLoc16 = 0x10000000  ; /* RamLoc16 */  
  __base_RAM = 0x10000000 ; /* RAM */  
  __top_RamLoc16 = 0x10000000 + 0x4000 ; /* 16K bytes */  
  __top_RAM = 0x10000000 + 0x4000 ; /* 16K bytes */  
