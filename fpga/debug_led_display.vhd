library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debug_led_display is
    port ( clk : in  std_logic;
           testpoint : in  std_logic;
           leds : out  std_logic_vector (7 downto 0));
end debug_led_display;

architecture Behavioral of debug_led_display is
    signal testpoint_reg : std_logic;
    signal edge_tick : std_logic;
    signal counter_reg, counter_next : unsigned(7 downto 0);
begin
    
    -- edge detection, from page 117 and 123 of the book, the tick will tell
    -- the counter whether or not it should count up or stay the same
    process (clk)
    begin
        if rising_edge(clk) then
            testpoint_reg <= testpoint;
        end if;
    end process;
    edge_tick <= (not testpoint_reg) and testpoint;

    -- counter
    process (clk)
    begin
        if rising_edge(clk) then
            counter_reg <= counter_next;
        end if;
    end process;
    
    -- next state logic for counter, only count up when the edge is detected
    counter_next <= 
        counter_reg + 1 when edge_tick = '1' else
        counter_reg;
    
    -- output
    leds <= std_logic_vector(counter_reg);

end Behavioral;
