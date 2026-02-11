library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity test_generator is
    Port ( clk : in  std_logic;
           data_ready : out  std_logic);
end test_generator;

architecture Behavioral of test_generator is
    -- use 4 for normal speed, use 16 for slow speed and debugging
    constant WIDTH : integer := 4;
    signal counter_reg, counter_next : unsigned(WIDTH-1 downto 0);
begin
    process (clk)
    begin
        if rising_edge(clk) then
            counter_reg <= counter_next;
        end if;
    end process;
    
    counter_next <= counter_reg + 1;
    
    -- use the MSB as our generated signal
    data_ready <= counter_reg(WIDTH-1);

end Behavioral;

