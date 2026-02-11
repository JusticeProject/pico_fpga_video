library ieee;
use ieee.std_logic_1164.all;

entity rgb_decoder is
    port ( clk : in  std_logic;
           reset : in  std_logic;
           video_on : in  std_logic;
           green_blue : in  std_logic_vector (1 downto 0);
           rgb : out  std_logic_vector (2 downto 0));
end rgb_decoder;

architecture Behavioral of rgb_decoder is
    signal red : std_logic;
    signal rgb_reg, rgb_next : std_logic_vector(2 downto 0);
begin
    -- grab the data on the falling edge because otherwise lines on the monitor look blurry
    process (reset, clk)
    begin
        if reset = '1' then
            rgb_reg <= "000";
        elsif falling_edge(clk) then
            rgb_reg <= rgb_next;
        end if;
    end process;

    -- next state logic
    red <= green_blue(1) and green_blue(0);
    rgb_next <= red & green_blue;
    
    -- output logic
    rgb <= rgb_reg when video_on = '1' else
        "000";
    
    --rgb <= 
    --    red & green_blue when video_on = '1' else
    --    "000";

end Behavioral;

