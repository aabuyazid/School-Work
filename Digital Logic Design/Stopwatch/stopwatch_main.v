`timescale 1ns / 1ps

module stopwatch_main(
    input clk,
    input reset,
    input start_stop,
    input [1:0] mode,
    input [7:0] ext_load,
    output [3:0] an,
    output [6:0] sseg,
    output dp
    );
    
    wire [15:0] digits;
    wire [6:0] tens, ones, tenths, hundredths;
    
    wire [13:0] dec_ext_load, dec_counter;
    
    wire slow_clk;
    
    // Module instantstation of clk_div_disp
    clk_div_disp a1 (.clk(clk), .slow_clk(slow_clk));
    
    bin2dec e1 (.binvalue(ext_load), .decvalue(dec_ext_load));
    
    stopwatch_states f1 (.clk(slow_clk), .reset(reset), .start_stop(start_stop), 
        .mode(mode), .ext_load(dec_ext_load), .counter(dec_counter));
        
    dec2bin g1 (.decvalue(dec_counter), .binvalue(digits));
    
    hexto7segment c1 (.x(digits[15:12]), .r(tens));
    hexto7segment c2 (.x(digits[11:8]), .r(ones));
    hexto7segment c3 (.x(digits[7:4]), .r(tenths));
    hexto7segment c4 (.x(digits[3:0]), .r(hundredths));
    
    stopwatch_display d1 (.clk(slow_clk), .in0(tens), .in1(ones), .in2(tenths), 
        .in3(hundredths), .an(an), .sseg(sseg), .dp(dp));
        
endmodule
