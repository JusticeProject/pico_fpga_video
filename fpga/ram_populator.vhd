library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ram_populator is
    port ( clk : in  std_logic;
           green : in  std_logic;
           blue : in  std_logic;
           data_ready : in  std_logic;
           start : in  std_logic;
           we : out  std_logic;
           green_blue : out  std_logic_vector (1 downto 0);
           addr : out  std_logic_vector (16 downto 0)
          );
end ram_populator;

-- see finite state machine diagram for details
architecture Behavioral of ram_populator is
    type state_type is (idle, capturing, captured);
    signal state_reg, state_next : state_type;
    signal x_reg, x_next : unsigned(8 downto 0);
    signal y_reg, y_next : unsigned(7 downto 0);
    
    constant X_WIDTH : integer := 320;
    constant Y_WIDTH : integer := 240;
begin
    -- state and data registers
    process (clk)
    begin
        if rising_edge(clk) then
            state_reg <= state_next;
            x_reg <= x_next;
            y_reg <= y_next;
        end if;
    end process;

    -- next state logic and data path
    process (state_reg, start, data_ready, x_reg, y_reg)
    begin
        state_next <= state_reg; -- default back to same state
        x_next <= x_reg;
        y_next <= y_reg;
        we <= '0';
        case state_reg is
            when idle =>
                if start = '1' then
                    x_next <= (others=>'0');
                    y_next <= (others=>'0');
                elsif data_ready = '1' then
                    state_next <= capturing;
                end if;
            when capturing =>
                we <= '1';
                state_next <= captured;
            when captured =>
                if data_ready = '0' then
                    state_next <= idle;
                    if x_reg >= (X_WIDTH - 1) then
                        x_next <= (others=>'0');
                        y_next <= y_reg + 1;
                    else
                        x_next <= x_reg + 1;
                    end if;
                end if;
            when others =>
                state_next <= idle;
        end case;
    end process;
    
    -- output logic
    green_blue <= green & blue;
    addr <= std_logic_vector(y_reg) & std_logic_vector(x_reg);
    
end Behavioral;

