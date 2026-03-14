module ram_populator(
    input wire clk,
    input wire green,
    input wire blue,
    input wire data_ready,
    input wire start,
    output reg we,
    output wire [1:0] green_blue,
    output wire [16:0] addr
    );

// signals
reg [1:0] state_reg, state_next;
reg [8:0] x_reg, x_next;
reg [7:0] y_reg, y_next;

// constants
localparam X_WIDTH = 320;

// states in the state machine
localparam [1:0]
    idle      = 2'b00,
    capturing = 2'b01,
    captured  = 2'b10;

// See Finite State Machine diagram for details.
// State and data registers
always @(posedge clk)
begin
    state_reg <= state_next;
    x_reg <= x_next;
    y_reg <= y_next;
end

// next-state logic and data path
always @*
begin
    state_next = state_reg; // default back to same state
    x_next = x_reg;
    y_next = y_reg;
    we = 1'b0;
    
    case (state_reg)
        idle:
            if (start)
                begin
                    x_next = 0;
                    y_next = 0;
                end
            else if (data_ready)
                state_next = capturing;
        capturing:
            begin
                we = 1'b1;
                state_next = captured;
            end
        captured:
            if (~data_ready)
                begin
                    state_next = idle;
                    if (x_reg >= (X_WIDTH - 1))
                        begin
                            x_next = 0;
                            y_next = y_reg + 1;
                        end
                    else
                        x_next = x_reg + 1;
                end
        default:
            state_next = idle;
    endcase
end

// output logic
assign green_blue = {green, blue};
assign addr = {y_reg, x_reg};

endmodule
