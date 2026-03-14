module dual_port_ram
#(
    parameter ADDR_WIDTH = 17,
              DATA_WIDTH = 2
)
(
    input wire clk,
    input wire we,
    input wire [ADDR_WIDTH-1:0] addr_w,
    input wire [DATA_WIDTH-1:0] data_w,
    input wire [ADDR_WIDTH-1:0] addr_r,
    output wire [DATA_WIDTH-1:0] data_r
);

// signal declarations
(* rw_addr_collision = "yes" *) reg [DATA_WIDTH-1:0] ram [(2**ADDR_WIDTH)-1:0];
reg [ADDR_WIDTH-1:0] addr_r_reg;

always @(posedge clk)
begin
    if (we)
        ram[addr_w] <= data_w;
    addr_r_reg <= addr_r;
end

assign data_r = ram[addr_r_reg];

endmodule
