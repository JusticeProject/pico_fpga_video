--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:58:37 01/25/2026
-- Design Name:   
-- Module Name:   C:/Users/Scott/Documents/Xilinx_Projects/vga_test/vga_simulation.vhd
-- Project Name:  vga_test
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: vga_sync
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY vga_simulation IS
END vga_simulation;
 
ARCHITECTURE behavior OF vga_simulation IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT vga_sync
    PORT(
         clk : IN  std_logic;
         reset : IN  std_logic;
         hsync : OUT  std_logic;
         vsync : OUT  std_logic;
         video_on : OUT  std_logic;
         p_tick : OUT  std_logic;
         pixel_x : OUT  std_logic_vector(9 downto 0);
         pixel_y : OUT  std_logic_vector(9 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal reset : std_logic := '0';

 	--Outputs
   signal hsync : std_logic;
   signal vsync : std_logic;
   signal video_on : std_logic;
   signal p_tick : std_logic;
   signal pixel_x : std_logic_vector(9 downto 0);
   signal pixel_y : std_logic_vector(9 downto 0);

   -- Clock period definitions
   constant clk_period : time := 20 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: vga_sync PORT MAP (
          clk => clk,
          reset => reset,
          hsync => hsync,
          vsync => vsync,
          video_on => video_on,
          p_tick => p_tick,
          pixel_x => pixel_x,
          pixel_y => pixel_y
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 40 ns.
      reset <= '1';
      --hsync <= '1';
      --vsync <= '1';
      --video_on <= '0';
      --p_tick <= '0';
      --pixel_x <= (others=>'0');
      --pixel_y <= (others=>'0');
      wait for 40 ns;
      reset <= '0';

      wait for clk_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
