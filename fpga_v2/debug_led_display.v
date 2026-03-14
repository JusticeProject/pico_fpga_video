
module debug_led_display(
    input wire clk,
    input wire testpoint,
    output wire [7:0] leds
    );

// signals
reg testpoint_reg;
wire edge_tick;
reg [7:0] counter_reg;
wire [7:0] counter_next;

// edge detection, the tick will tell the counter whether it should count up or stay the same
always @(posedge clk)
    testpoint_reg <= testpoint;
assign edge_tick = (~testpoint_reg) && testpoint;

// counter
always @(posedge clk)
    counter_reg <= counter_next;

// next-state logic for counter, only count up when edge is detected
assign counter_next = (edge_tick) ? (counter_reg + 1) : counter_reg;

// output logic
assign leds = counter_reg;

endmodule
