--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : video_card.vhf
-- /___/   /\     Timestamp : 02/11/2026 15:37:17
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -intstyle ise -family spartan3e -flat -suppress -vhdl C:/Users/Scott/Documents/Xilinx_Projects/video_card/video_card.vhf -w C:/Users/Scott/Documents/Xilinx_Projects/video_card/video_card.sch
--Design Name: video_card
--Device: spartan3e
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity video_card is
   port ( blue_async       : in    std_logic; 
          clk              : in    std_logic; 
          data_ready_async : in    std_logic; 
          green_async      : in    std_logic; 
          reset            : in    std_logic; 
          start_async      : in    std_logic; 
          hsync            : out   std_logic; 
          leds             : out   std_logic_vector (7 downto 0); 
          rgb              : out   std_logic_vector (2 downto 0); 
          vsync            : out   std_logic);
end video_card;

architecture BEHAVIORAL of video_card is
   signal XLXN_6           : std_logic_vector (16 downto 0);
   signal XLXN_11          : std_logic;
   signal XLXN_13          : std_logic_vector (1 downto 0);
   signal XLXN_14          : std_logic;
   signal XLXN_15          : std_logic_vector (16 downto 0);
   signal XLXN_16          : std_logic_vector (1 downto 0);
   signal XLXN_21          : std_logic;
   signal XLXN_22          : std_logic;
   signal XLXN_23          : std_logic;
   signal XLXN_24          : std_logic;
   component vga_sync
      port ( clk      : in    std_logic; 
             reset    : in    std_logic; 
             hsync    : out   std_logic; 
             vsync    : out   std_logic; 
             video_on : out   std_logic; 
             addr     : out   std_logic_vector (16 downto 0));
   end component;
   
   component rgb_decoder
      port ( clk        : in    std_logic; 
             reset      : in    std_logic; 
             video_on   : in    std_logic; 
             green_blue : in    std_logic_vector (1 downto 0); 
             rgb        : out   std_logic_vector (2 downto 0));
   end component;
   
   component dual_port_ram
      port ( clk    : in    std_logic; 
             we     : in    std_logic; 
             addr_w : in    std_logic_vector (16 downto 0); 
             data_w : in    std_logic_vector (1 downto 0); 
             addr_r : in    std_logic_vector (16 downto 0); 
             data_r : out   std_logic_vector (1 downto 0));
   end component;
   
   component ram_populator
      port ( clk        : in    std_logic; 
             green      : in    std_logic; 
             blue       : in    std_logic; 
             data_ready : in    std_logic; 
             start      : in    std_logic; 
             we         : out   std_logic; 
             green_blue : out   std_logic_vector (1 downto 0); 
             addr       : out   std_logic_vector (16 downto 0));
   end component;
   
   component synchronizer_2_stage
      port ( clk   : in    std_logic; 
             async : in    std_logic; 
             sync  : out   std_logic);
   end component;
   
   component debug_led_display
      port ( clk       : in    std_logic; 
             testpoint : in    std_logic; 
             leds      : out   std_logic_vector (7 downto 0));
   end component;
   
begin
   XLXI_1 : vga_sync
      port map (clk=>clk,
                reset=>reset,
                addr(16 downto 0)=>XLXN_6(16 downto 0),
                hsync=>hsync,
                video_on=>XLXN_11,
                vsync=>vsync);
   
   XLXI_2 : rgb_decoder
      port map (clk=>clk,
                green_blue(1 downto 0)=>XLXN_13(1 downto 0),
                reset=>reset,
                video_on=>XLXN_11,
                rgb(2 downto 0)=>rgb(2 downto 0));
   
   XLXI_3 : dual_port_ram
      port map (addr_r(16 downto 0)=>XLXN_6(16 downto 0),
                addr_w(16 downto 0)=>XLXN_15(16 downto 0),
                clk=>clk,
                data_w(1 downto 0)=>XLXN_16(1 downto 0),
                we=>XLXN_14,
                data_r(1 downto 0)=>XLXN_13(1 downto 0));
   
   XLXI_5 : ram_populator
      port map (blue=>XLXN_22,
                clk=>clk,
                data_ready=>XLXN_23,
                green=>XLXN_21,
                start=>XLXN_24,
                addr(16 downto 0)=>XLXN_15(16 downto 0),
                green_blue(1 downto 0)=>XLXN_16(1 downto 0),
                we=>XLXN_14);
   
   XLXI_6 : synchronizer_2_stage
      port map (async=>green_async,
                clk=>clk,
                sync=>XLXN_21);
   
   XLXI_7 : synchronizer_2_stage
      port map (async=>blue_async,
                clk=>clk,
                sync=>XLXN_22);
   
   XLXI_8 : synchronizer_2_stage
      port map (async=>data_ready_async,
                clk=>clk,
                sync=>XLXN_23);
   
   XLXI_9 : synchronizer_2_stage
      port map (async=>start_async,
                clk=>clk,
                sync=>XLXN_24);
   
   XLXI_11 : debug_led_display
      port map (clk=>clk,
                testpoint=>XLXN_24,
                leds(7 downto 0)=>leds(7 downto 0));
   
end BEHAVIORAL;


