module top(
    input clk,
    input reset,
    input start_async,
    input data_ready_async,
    input blue_async,
    input green_async,
    output hsync,
    output vsync,
    output [2:0] rgb,
    output [7:0] leds
    );

// !!**!!**
// TODO: need to generate 50MHz clock if using FPGA board that is programmed with Vivado
// !!**!!**

// signals
wire start_sync, data_ready_sync, blue_sync, green_sync;
wire we;
wire [16:0] addr_w, addr_r;
wire [1:0] data_w, data_r;
wire video_on;

// instantiate input signal handling
synchronizer_2_stage synchronizer_2_stage_start (.clk(clk), .async(start_async), .sync(start_sync));
synchronizer_2_stage synchronizer_2_stage_data_ready (.clk(clk), .async(data_ready_async), .sync(data_ready_sync));
synchronizer_2_stage synchronizer_2_stage_blue (.clk(clk), .async(blue_async), .sync(blue_sync));
synchronizer_2_stage synchronizer_2_stage_green (.clk(clk), .async(green_async), .sync(green_sync));

// instantiate the led debug display
debug_led_display debug_led_display_unit (.clk(clk), .testpoint(start_sync), .leds(leds));

// instantiate memory components
ram_populator ram_populator_unit (.clk(clk), .green(green_sync), .blue(blue_sync), 
    .data_ready(data_ready_sync), .start(start_sync), .we(we), .green_blue(data_w), .addr(addr_w));
dual_port_ram dual_port_ram_unit (.clk(clk), .we(we), .addr_w(addr_w), .data_w(data_w), .addr_r(addr_r), .data_r(data_r));

// instantiate video handling
vga_sync vga_sync_unit (.clk(clk), .reset(reset), .hsync(hsync), .vsync(vsync), .video_on(video_on), .addr(addr_r));
rgb_decoder rgb_decoder_unit (.clk(clk), .reset(reset), .video_on(video_on), .green_blue(data_r), .rgb(rgb));
  
endmodule
