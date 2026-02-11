library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

entity dual_port_ram is
    generic(
      ADDR_WIDTH: integer:=17;
      DATA_WIDTH: integer:=2
    );
   
    port ( clk : in  std_logic;
           we : in  std_logic;
           addr_w : in  std_logic_vector (ADDR_WIDTH-1 downto 0);
           data_w : in  std_logic_vector (DATA_WIDTH-1 downto 0);
           addr_r : in  std_logic_vector (ADDR_WIDTH-1 downto 0);
           data_r : out  std_logic_vector (DATA_WIDTH-1 downto 0)
           );
end dual_port_ram;

architecture Behavioral of dual_port_ram is
    type ram_type is array (0 to (2**ADDR_WIDTH)-1) of std_logic_vector(DATA_WIDTH-1 downto 0);
    signal ram : ram_type;
    signal addr_r_reg: std_logic_vector(ADDR_WIDTH-1 downto 0);
begin
    process (clk)
    begin
        if (rising_edge(clk)) then
            if (we = '1') then
                ram(to_integer(unsigned(addr_w))) <= data_w;
            end if;
            addr_r_reg <= addr_r;
        end if;
   end process;
   
   data_r <= ram(to_integer(unsigned(addr_r_reg)));

end Behavioral;
